#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include <vector>
#include <d3dx9.h>

#include <GWCA/GWCA.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Constants/Constants.h>

#include <GWCA/GameEntities/Map.h>
#include <GWCA/GameEntities/Pathing.h>
#include <GWCA/GameEntities/Agent.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/MapContext.h>

#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/RenderMgr.h>
struct IDirect3DDevice9;

namespace GW {
	typedef struct {
		GamePos pos;
		const PathingTrapezoid* t;
	} PathPoint;
	//typedef void(__cdecl* FindPath_pt)(void* MapCtxSub1, PathPoint* start, PathPoint* goal, int unk, uint32_t maxCount, uint32_t* count, PathPoint* pathArray);
	//typedef void(__cdecl* FindPath_pt)(void* MapCtxSub1, PathPoint* start, PathPoint* goal, float unk, uint32_t maxCount, uint32_t* count, PathPoint* pathArray); //typedef void(__cdecl* FindPath_pt)(void* MapCtxSub1, GamePos* start, GamePos* goal, float unk, uint32_t maxCount, uint32_t* count, PathPoint* pathArray);
	typedef void(__cdecl* FindPath_pt)(PathPoint* start, PathPoint* goal, float range, uint32_t maxCount, uint32_t* count, PathPoint* pathArray);
	static FindPath_pt FindPath_Func = nullptr;

	static void InitPathfinding() {
		//FindPath_Func = (FindPath_pt)GW::Scanner::Find("\x50\x8b\x45\x14\xff\x75\x08\xff\x70\x74", "xxxxxxxxxx", -0x12a);
		//FindPath_Func = (FindPath_pt)GW::Scanner::Find("\x83\xc4\x14\x8d\x77\x30\x90\x83\x7e\x0c\x00", "xxxxxxxxxx", -0xd9);
		FindPath_Func = (FindPath_pt)GW::Scanner::Find("\x83\xec\x20\x53\x8b\x5d\x1c\x56\x57\xe8\x2f\x4b", "xxxxxxxxxxxx", -0x3);
		if (!FindPath_Func) {
			//todo: handle error
		}
	}
}


class DxDraw {
	const uint32_t CUSTOMFVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE);
	struct CUSTOMVERTEX {
		FLOAT x, y, z;    // from the D3DFVF_XYZRHW flag
		DWORD color;    // from the D3DFVF_DIFFUSE flag
	};

	LPDIRECT3DVERTEXBUFFER9 m_vbuffer;
	IDirect3DDevice9* m_device;
	int m_vertex_count;
	std::vector<CUSTOMVERTEX> m_vertices;
	DWORD m_color;

public:
	float m_scale;

	DxDraw(IDirect3DDevice9* device, int vertex_count) :
		m_device(device), m_vertex_count(vertex_count),
		m_color(0), m_vbuffer(), m_scale(50.0f) {
		m_device->CreateVertexBuffer(m_vertex_count * sizeof(CUSTOMVERTEX),
			D3DUSAGE_WRITEONLY,
			CUSTOMFVF,
			D3DPOOL_MANAGED,
			&m_vbuffer,
			NULL);
		m_vertices.reserve(m_vertex_count);
	}
	~DxDraw() {
		m_vbuffer->Release();
	}

	void setColor(uint32_t rgba) {
		m_color = rgba;
	}

	void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff) {
		m_color = D3DCOLOR_RGBA(r, g, b, a);
	}

	void drawLine(const GW::Vec3f& from, const GW::Vec3f& to) {
		if (m_vertex_count - m_vertices.size() < 2) {
			//Too many vertices
			return;
		}
		m_vertices.push_back(CUSTOMVERTEX{ from.x, from.y, from.z, m_color });
		m_vertices.push_back(CUSTOMVERTEX{ to.x, to.y, to.z, m_color });
	}

	void render() {
		if (m_vertices.size() == 0)
			return;

		DWORD dwOld_D3DRS_COLORWRITEENABLE;
		m_device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE); /*this is so we can draw without net_graph*/
		m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		m_device->SetRenderState(D3DRS_LIGHTING, FALSE);

		auto R0 = *GW::Render::GetTransform((GW::Render::Transform)0);
		//This projection transform doesn't work when there is opened map.
		D3DXMATRIX matProjection(
			R0._11 / R0._33, 0.0f, 0.0f, 0.0f,
			0.0f, R0._22 / R0._33, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0001f, 1.0f,
			0.0f, 0.0f, -10.001f, 0.0f);    // the projection transform matrix
		m_device->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection transform

		D3DXMATRIX matWord;    // the wts transform matrix
		D3DXMatrixIdentity(&matWord);
		auto R1 = *GW::Render::GetTransform((GW::Render::Transform)1);
		matWord._11 = R1._11;
		matWord._21 = R1._12;
		matWord._31 = R1._13;
		matWord._41 = R1._14;
		matWord._12 = R1._21;
		matWord._22 = R1._22;
		matWord._32 = R1._23;
		matWord._42 = R1._24;
		matWord._13 = R1._31;
		matWord._23 = R1._32;
		matWord._33 = R1._33;
		matWord._43 = R1._34;
		m_device->SetTransform(D3DTS_WORLD, &matWord);

		// select which vertex format we are using
		m_device->SetFVF(CUSTOMFVF);
		// select the vertex buffer to display
		m_device->SetStreamSource(0, m_vbuffer, 0, sizeof(CUSTOMVERTEX));
		// copy the vertex buffer to the back buffer
		m_device->DrawPrimitive(D3DPT_LINELIST, 0, m_vertices.size() / 2);

		VOID* pVoid;    // the void pointer
		m_vbuffer->Lock(0, m_vertices.size() * sizeof(CUSTOMVERTEX), (void**)&pVoid, D3DLOCK_DISCARD);    // lock the vertex buffer
		memcpy(pVoid, m_vertices.data(), m_vertices.size() * sizeof(CUSTOMVERTEX));    // copy the vertices to the locked buffer
		m_vbuffer->Unlock();    // unlock the vertex buffer
		m_vertices.clear();

		m_device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	}
};

static std::unique_ptr<DxDraw> dxdraw;
static void DrawLineInRange(GW::Vec3f from, GW::Vec3f to, float sqrange = 15000 * 15000) {
	if (GetSquaredNorm(from) < sqrange && GetSquaredNorm(to) < sqrange) {
		dxdraw->drawLine(from, to);
	}
}

static void DrawPathingTrapezoid(const GW::PathingTrapezoid& t, uint32_t layer, bool unpassable = false) {
	using namespace GW;

	float altitude = 0;
	GamePos gp{ t.XBL, t.YB, layer };
	GW::Map::QueryAltitude(gp, 10, altitude);
	Vec3f A{ t.XBL, t.YB, altitude };

	gp = { t.XBR, t.YB, layer };
	GW::Map::QueryAltitude(gp, 10, altitude);
	Vec3f B{ t.XBR, t.YB, altitude };

	gp = { t.XTR, t.YT, layer };
	GW::Map::QueryAltitude(gp, 10, altitude);
	Vec3f C{ t.XTR, t.YT, altitude };

	gp = { t.XTL, t.YT, layer };
	GW::Map::QueryAltitude(gp, 10, altitude);
	Vec3f D{ t.XTL, t.YT, altitude };

	if (unpassable) {
		dxdraw->setColor(255, 0, 0);
		DrawLineInRange(B, C);
		DrawLineInRange(D, A);
		DrawLineInRange(C, D);
		DrawLineInRange(A, B);
		DrawLineInRange(A, C);
		DrawLineInRange(B, D);
	}
	else {
		dxdraw->setColor(layer * 255, 255 - layer * 6, layer * 6);
		DrawLineInRange(B, C);
		DrawLineInRange(D, A);
		dxdraw->setColor(layer * 255, 255 - layer * 6, layer * 6, 20);
		DrawLineInRange(C, D);
		DrawLineInRange(A, B);
	}
}

static void DrawPathingTrapezoids() {
	using namespace GW;
	const PathingMapArray& map = Map::GetPathingMap();
	GW::GameContext* g = GW::GameContext::instance();
	GW::MapContext* mapContext = g->map;
	GW::Array<uint32_t> sub1s = mapContext->sub1->pathing_map_block;
	for (uint32_t i = 0; i < map.size(); i++) {
		const auto& m = map[i];
		for (uint32_t j = 0; j < m.trapezoid_count; j++) {
			const auto& t = m.trapezoids[j];
			DrawPathingTrapezoid(t, i, sub1s[i]);
		}
	}
}

static void DrawPaths() {
	using namespace GW;
	GW::AgentLiving* me = GW::Agents::GetCharacter();
	GW::AgentArray agents = GW::Agents::GetAgentArray();
	if (!me || !agents.valid()) return;
	dxdraw->setColor(20, 240, 200);

	PathPoint pathArray[30];
	PathPoint start{ me->pos, nullptr };

	for (unsigned int i = 0; i < agents.size(); ++i) {
		if (agents[i] == nullptr) continue;
		GW::AgentLiving* agent = agents[i]->GetAsAgentLiving();
		if (agent == nullptr) continue;

		//For some reason GW shits itself if you try to find path to a point farther than 5000 units away by air distance,
		//despite GW client being able to call it with 10000 units.
		if (GetSquareDistance(me->pos, agent->pos) > 4500 * 4500) continue;

		PathPoint goal{ agent->pos, nullptr };
		uint32_t pathCount = sizeof(pathArray);
		FindPath_Func(&start, &goal, 10000.0f, pathCount, &pathCount, pathArray);

		float altitude = 0;
		GamePos g1 = pathArray[0].pos;		
		GW::Map::QueryAltitude(g1, 10, altitude);
		Vec3f p1{ g1.x, g1.y, altitude };
		DrawLineInRange(p1, Vec3f(me->pos.x, me->pos.y, me->z));

		for (uint32_t i = 1; i < pathCount; i++) {
			g1 = pathArray[i].pos;
			GW::Map::QueryAltitude(g1, 10, altitude);
			Vec3f p2{ g1.x, g1.y, altitude };
			DrawLineInRange(p1, p2);
			p1 = p2;
		}
	}
}

static volatile bool running;

static void Pathing_Draw(IDirect3DDevice9 *device)
{
    // This is call from within the game thread and all operation should be done here.
    // You can't freeze this thread, so no blocking operation or at your own risk.
    static bool initialized = false;
    if (!initialized) {
		GW::InitPathfinding();
		dxdraw.reset(new DxDraw(device, 1000000));
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Pathing info: Initialized");
        initialized = true;
    }

	// Sanity checks; context not ready.
	if (GW::Map::GetInstanceType() == GW::Constants::InstanceType::Loading) return;
	GW::GameContext* gameContext = GW::GameContext::instance();
	if (!gameContext || !gameContext->map) return; 

	DrawPathingTrapezoids();
	DrawPaths();
	dxdraw->render();
}

static DWORD WINAPI ThreadProc(LPVOID lpModule)
{
    // This is a new thread so you should only initialize GWCA and setup the hook on the game thread.
    // When the game thread hook is setup (i.e. SetRenderCallback), you should do the next operations
    // on the game from within the game thread.

    HMODULE hModule = static_cast<HMODULE>(lpModule);

    GW::Initialize();

    GW::Render::SetRenderCallback(Pathing_Draw);

    running = true;
    while (running) {
        Sleep(100);
    }

    // Hooks are disable from Guild Wars thread (safely), so we just make sure we exit the last hooks
    while (GW::HookBase::GetInHookCount())
        Sleep(16);

    // We can't guarantee that the code in Guild Wars thread isn't still in the trampoline, but
    // practically a short sleep is fine.
    Sleep(16);
    GW::Terminate();

    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    DisableThreadLibraryCalls(hModule);

    if (dwReason == DLL_PROCESS_ATTACH) {
        HANDLE handle = CreateThread(0, 0, ThreadProc, hModule, 0, 0);
        CloseHandle(handle);
    }

    return TRUE;
}
