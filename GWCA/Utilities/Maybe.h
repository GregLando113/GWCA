#pragma once

namespace GW {
    template<typename T>
    class Maybe {
    public:
        constexpr Maybe<T>() : hasValue(false), value(T()) {}
        Maybe<T>(Maybe<T>&& value) : hasValue(std::move(hasValue)), value(std::move(value)) {}
        Maybe<T>(const Maybe<T>& value) : hasValue(value.hasValue), value(value.value) {}
        Maybe<T>(T&& value) : hasValue(true), value(std::move(value)) {}
        Maybe<T>(const T& value) : hasValue(true), value(value) {}

        Maybe<T>& operator=(Maybe<T>&& rhs) {
            hasValue = std::move(rhs.hasValue);
            value = std::move(rhs.value);
            return *this;
        }

        Maybe<T>& operator=(const Maybe<T>& rhs) {
            hasValue = rhs.hasValue;
            value = T(rhs.value);
            return *this;
        }

        Maybe<T>& operator=(T&& rhs) {
            hasValue = true;
            value = std::move(rhs);
            return *this;
        }

        Maybe<T>& operator=(const T& rhs) {
            hasValue = true;
            value = T(rhs);
            return *this;
        }

        bool operator==(const Maybe<T>& rhs) const {
            if (hasValue && rhs.hasValue)
                return value == rhs.value;

            return hasValue == rhs.hasValue;
        }

        bool operator==(const T& rhs) const {
            return hasValue && (value == rhs);
        }

        bool operator!=(const Maybe<T>& rhs) const {
            return !operator==(rhs);
        }

        bool operator!=(const T& rhs) const {
            return !operator==(rhs);
        }

        bool HasValue() const {
            return hasValue;
        }

        const T& Value() const {
            if (hasValue)
                return value;

            throw std::exception("No value");
        }

    private:
        bool hasValue;
        T value;
    };

    template<typename T> Maybe<T> Just(T&& value) { return Maybe<T>(value); }
    template<typename T> Maybe<T> Just(const T& value) { return Maybe<T>(value); }
    template<typename T> Maybe<T> Nothing() { return Maybe<T>(); }
}