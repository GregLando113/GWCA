#pragma once

#ifdef GWCA_IMPORT
#if defined(__clang__) || defined(__GNUC__)
#define GWCA_API __attribute__((dllimport))
#else
#define GWCA_API __declspec(dllimport)
#endif
#else 
#if defined(__clang__) || defined(__GNUC__)
#define GWCA_API __attribute__((dllexport))
#else
#define GWCA_API __declspec(dllexport)
#endif
#endif