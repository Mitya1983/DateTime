#include "time.hpp"
#include <chrono>
#include <algorithm>

namespace{
    auto checkTimeFormat(const std::string& time) -> bool;
    const uint32_t minutes_in_day = 1440;
    const uint32_t seconds_in_day = 86400;
    const uint32_t milliseconds_in_day = 86400000;
    const uint64_t microseconds_in_day = 86400000000;
    const uint64_t nanoseconds_in_day = 86400000000000;
    const uint32_t minutes_in_hour = 60;
    const uint32_t seconds_in_hour = 3600;
    const uint32_t milliseconds_in_hour = 3600000;
    const uint64_t microseconds_in_hour = 3600000000;
    const uint64_t nanoseconds_in_hour = 3600000000000;
    const uint32_t seconds_in_minute = 60;
    const uint32_t milliseconds_in_minute = 60000;
    const uint64_t microseconds_in_minute = 60000000;
    const uint64_t nanoseconds_in_minute = 60000000000;
    const uint32_t milliseconds_in_second = 1000;
    const uint64_t microseconds_in_second = 1000000;
    const uint64_t nanoseconds_in_second = 1000000000;
    const uint64_t microseconds_in_millisecond = 1000;
    const uint64_t nanoseconds_in_millisecond = 1000000;
    const uint64_t nanoseconds_in_microsecond = 1000;
    using Days = std::chrono::duration<int64_t, std::ratio_divide<std::ratio<seconds_in_day>, std::chrono::seconds::period>>;
} //End of unnamed namespace

tristan::time::Time::Time(tristan::time::Precision precision) :
        m_precision{precision},
        m_offset{tristan::time::TimeZone::UTC}{
    auto time_point = std::chrono::system_clock::duration(std::chrono::system_clock::now().time_since_epoch());
    auto days = std::chrono::duration_cast<Days>(time_point);

    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            m_time_since_day_start = std::chrono::duration_cast<std::chrono::minutes>(
                    time_point - std::chrono::duration_cast<std::chrono::system_clock::duration>(days)
            );
            break;
        }
        case tristan::time::Precision::SECONDS:{
            m_time_since_day_start = std::chrono::duration_cast<std::chrono::seconds>(
                    time_point - std::chrono::duration_cast<std::chrono::system_clock::duration>(days)
            );
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            m_time_since_day_start = std::chrono::duration_cast<std::chrono::milliseconds>(
                    time_point - std::chrono::duration_cast<std::chrono::system_clock::duration>(days)
            );
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            m_time_since_day_start = std::chrono::duration_cast<std::chrono::microseconds>(
                    time_point - std::chrono::duration_cast<std::chrono::system_clock::duration>(days)
            );
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            m_time_since_day_start = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    time_point - std::chrono::duration_cast<std::chrono::system_clock::duration>(days)
            );
            break;
        }
    }
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes) noexcept(false):
        m_precision{tristan::time::Precision::MINUTES},
        m_offset{tristan::time::TimeZone::UTC}{
    if (hours > 23){
        std::string message =
                "tristan::time::Time(int hours, int minutes, int seconds): bad [hour] value was provided - "
                + std::to_string(hours) + ". The value from 0 to 23 is expected";
        throw std::range_error{message};
    }
    if (minutes > 59){
        std::string message =
                "tristan::time::Time(int hours, int minutes, int seconds): bad [minutes] value was provided - "
                + std::to_string(minutes) + ". The value from 0 to 59 is expected";
        throw std::range_error{message};
    }
    m_time_since_day_start = std::chrono::minutes{hours * 60 + minutes};
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds) :
        tristan::time::Time{hours, minutes}{
    if (seconds > 59){
        std::string message =
                "tristan::time::Time(int hours, int minutes, int seconds): bad [seconds] value was provided - "
                + std::to_string(seconds) + ". The value from 0 to 59 is expected";
        throw std::range_error{message};
    }
    auto l_minutes = std::get<std::chrono::minutes>(m_time_since_day_start);
    m_time_since_day_start =
            std::chrono::duration_cast<std::chrono::seconds>(l_minutes) + std::chrono::seconds{seconds};

    m_precision = tristan::time::Precision::SECONDS;
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) :
        tristan::time::Time{hours, minutes, seconds}{
    if (milliseconds > 999){
        std::string message =
                "tristan::time::Time(int hours, int minutes, int seconds, uint16_t milliseconds): bad [milliseconds] value was provided - "
                + std::to_string(seconds) + ". The value from 0 to 999 is expected";
        throw std::range_error{message};
    }
    auto l_seconds = std::get<std::chrono::seconds>(m_time_since_day_start);
    m_time_since_day_start =
            std::chrono::duration_cast<std::chrono::milliseconds>(l_seconds) + std::chrono::milliseconds{milliseconds};

    m_precision = tristan::time::Precision::MILLISECONDS;
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds,
                          uint16_t microseconds) :
        tristan::time::Time{hours, minutes, seconds, milliseconds}{
    if (microseconds > 999){
        std::string message =
                "tristan::time::Time(int hours, int minutes, int seconds, uint16_t milliseconds): bad [microseconds] value was provided - "
                + std::to_string(seconds) + ". The value from 0 to 999 is expected";
        throw std::range_error{message};
    }
    auto l_milliseconds = std::get<std::chrono::milliseconds>(m_time_since_day_start);
    m_time_since_day_start = std::chrono::duration_cast<std::chrono::microseconds>(l_milliseconds) +
                             std::chrono::microseconds{microseconds};

    m_precision = tristan::time::Precision::MICROSECONDS;
}

tristan::time::Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds,
                          uint16_t microseconds, uint16_t nanoseconds) :
        tristan::time::Time{hours, minutes, seconds, milliseconds, microseconds}{
    if (nanoseconds > 999){
        std::string message =
                "tristan::time::Time(int hours, int minutes, int seconds, uint16_t milliseconds): bad [nanoseconds] value was provided - "
                + std::to_string(seconds) + ". The value from 0 to 999 is expected";
        throw std::range_error{message};
    }
    auto l_microseconds = std::get<std::chrono::microseconds>(m_time_since_day_start);
    m_time_since_day_start = std::chrono::duration_cast<std::chrono::nanoseconds>(l_microseconds) +
                             std::chrono::nanoseconds{nanoseconds};

    m_precision = tristan::time::Precision::NANOSECONDS;
}

tristan::time::Time::Time(const std::string& time) :
        m_offset{tristan::time::TimeZone::UTC},
        m_precision(tristan::time::Precision::MINUTES){

    auto l_time = time;

    auto offset_pos = l_time.find_first_of("-+");
    auto offset = tristan::time::TimeZone::UTC;
    if (offset_pos != std::string::npos && offset_pos == l_time.size() - 3){
        offset = static_cast<tristan::time::TimeZone>(std::stoi(l_time.substr(offset_pos)));
        l_time.erase(offset_pos);
    }
    if (!checkTimeFormat(l_time)){
        throw std::invalid_argument{"tristan::time::Time::Time(const std::string& time): Invalid time format"};
    }
    auto size = l_time.length();

    const uint8_t hours_pos = 0;
    const uint8_t minutes_pos = 3;
    const uint8_t seconds_pos = 6;
    const uint8_t milliseconds_pos = 9;
    const uint8_t microseconds_pos = 13;
    const uint8_t nanoseconds_pos = 17;


    switch (size){
        case 5:{
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            *this = tristan::time::Time(hours, minutes);
            break;
        }
        case 8:{
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            *this = tristan::time::Time(hours, minutes, seconds);
            break;
        }
        case 12:{
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            auto milliseconds = std::stoi(l_time.substr(milliseconds_pos, 3));
            *this = tristan::time::Time(hours, minutes, seconds, milliseconds);
            break;
        }
        case 16:{
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            auto milliseconds = std::stoi(l_time.substr(milliseconds_pos, 3));
            auto microseconds = std::stoi(l_time.substr(microseconds_pos, 3));
            *this = tristan::time::Time(hours, minutes, seconds, milliseconds, microseconds);
            break;
        }
        case 20:{
            auto hours = std::stoi(l_time.substr(hours_pos, 2));
            auto minutes = std::stoi(l_time.substr(minutes_pos, 2));
            auto seconds = std::stoi(l_time.substr(seconds_pos, 2));
            auto milliseconds = std::stoi(l_time.substr(milliseconds_pos, 3));
            auto microseconds = std::stoi(l_time.substr(microseconds_pos, 3));
            auto nanoseconds = std::stoi(l_time.substr(nanoseconds_pos, 3));
            *this = tristan::time::Time(hours, minutes, seconds, milliseconds, microseconds, nanoseconds);
            break;
        }
        default:{
            throw std::invalid_argument{
                    "tristan::time::Time::Time(const std::string& time): Invalid time format"};
        }
    }
    if (m_offset != offset){
        m_offset = offset;
    }
}

auto tristan::time::Time::operator==(const tristan::time::Time& r) const -> bool{

    if (m_precision != r.m_precision){
        return false;
    }

    return m_time_since_day_start == r.m_time_since_day_start;
}

auto tristan::time::Time::operator<(const tristan::time::Time& r) const -> bool{
    if (m_precision != r.m_precision){
        return false;
    }

    return m_time_since_day_start < r.m_time_since_day_start;
}

void tristan::time::Time::operator+=(const tristan::time::Time& r){
    *this = *this + r;
}

void tristan::time::Time::operator-=(const tristan::time::Time& r){
    *this = *this - r;
}

void tristan::time::Time::addHours(uint64_t hours){

    if (hours == 0){
        return;
    }
    if (hours > 23){
        hours %= 24;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            _addMinutes(hours * minutes_in_hour);
            break;
        }
        case tristan::time::Precision::SECONDS:{
            _addSeconds(hours * seconds_in_hour);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            _addMilliseconds(hours * milliseconds_in_hour);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _addMicroseconds(hours * microseconds_in_hour);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _addNanoseconds(hours * nanoseconds_in_hour);
            break;
        }
    }
}

void tristan::time::Time::addMinutes(uint64_t minutes){
    if (minutes == 0){
        return;
    }
    if (minutes > minutes_in_day){
        minutes %= minutes_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            _addMinutes(minutes);
            break;
        }
        case tristan::time::Precision::SECONDS:{
            _addSeconds(minutes * seconds_in_minute);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            _addMilliseconds(minutes * milliseconds_in_minute);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _addMicroseconds(minutes * microseconds_in_minute);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _addNanoseconds(minutes * nanoseconds_in_minute);
            break;
        }
    }
}

void tristan::time::Time::addSeconds(uint64_t seconds){
    if (seconds == 0){
        return;
    }
    if (seconds > seconds_in_day){
        seconds %= seconds_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            if (seconds < seconds_in_minute){
                return;
            }
            _addMinutes(seconds % seconds_in_minute);
        }
        case tristan::time::Precision::SECONDS:{
            _addSeconds(seconds);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            _addMilliseconds(seconds * milliseconds_in_second);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _addMicroseconds(seconds * microseconds_in_second);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _addNanoseconds(seconds * nanoseconds_in_second);
            break;
        }
    }
}

void tristan::time::Time::addMilliseconds(uint64_t milliseconds){
    if (milliseconds == 0){
        return;
    }
    if (milliseconds > milliseconds_in_day){
        milliseconds %= milliseconds_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            if (milliseconds < milliseconds_in_minute){
                return;
            }
            _addMinutes(milliseconds % milliseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS:{
            if (milliseconds < milliseconds_in_second){
                return;
            }
            _addSeconds(milliseconds % milliseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            _addMilliseconds(milliseconds);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _addMicroseconds(milliseconds * microseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _addNanoseconds(milliseconds * nanoseconds_in_millisecond);
            break;
        }
    }
}

void tristan::time::Time::addMicroseconds(uint64_t microseconds){
    if (microseconds == 0){
        return;
    }
    if (microseconds > microseconds_in_day){
        microseconds %= microseconds_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            if (microseconds < microseconds_in_minute){
                return;
            }
            _addMinutes(microseconds % microseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS:{
            if (microseconds < microseconds_in_second){
                return;
            }
            _addSeconds(microseconds % microseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            if (microseconds < microseconds_in_millisecond){
                return;
            }
            _addMilliseconds(microseconds % microseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _addMicroseconds(microseconds);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _addNanoseconds(microseconds * nanoseconds_in_microsecond);
            break;
        }
    }
}

void tristan::time::Time::addNanoseconds(uint64_t nanoseconds){
    if (nanoseconds == 0){
        return;
    }
    if (nanoseconds > nanoseconds_in_day){
        nanoseconds %= nanoseconds_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            if (nanoseconds < nanoseconds_in_minute){
                return;
            }
            _addMinutes(nanoseconds % nanoseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS:{
            if (nanoseconds < nanoseconds_in_second){
                return;
            }
            _addSeconds(nanoseconds % nanoseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            if (nanoseconds < nanoseconds_in_millisecond){
                return;
            }
            _addMilliseconds(nanoseconds % nanoseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            if (nanoseconds < nanoseconds_in_microsecond){
                return;
            }
            _addMicroseconds(nanoseconds % nanoseconds_in_microsecond);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _addNanoseconds(nanoseconds);
            break;
        }
    }
}

void tristan::time::Time::subtractHours(uint64_t hours){
    if (hours == 0){
        return;
    }
    if (hours > 23){
        hours %= 24;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            _subtractMinutes(hours * minutes_in_hour);
            break;
        }
        case tristan::time::Precision::SECONDS:{
            _subtractSeconds(hours * seconds_in_hour);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            _subtractMilliseconds(hours * milliseconds_in_hour);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _subtractMicroseconds(hours * microseconds_in_hour);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _subtractNanoseconds(hours * nanoseconds_in_hour);
            break;
        }
    }
}

void tristan::time::Time::subtractMinutes(uint64_t minutes){
    if (minutes == 0){
        return;
    }
    if (minutes > minutes_in_day){
        minutes %= minutes_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            _subtractMinutes(minutes);
            break;
        }
        case tristan::time::Precision::SECONDS:{
            _subtractSeconds(minutes * seconds_in_minute);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            _subtractMilliseconds(minutes * milliseconds_in_minute);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _subtractMicroseconds(minutes * microseconds_in_minute);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _subtractNanoseconds(minutes * nanoseconds_in_minute);
            break;
        }
    }
}

void tristan::time::Time::subtractSeconds(uint64_t seconds){
    if (seconds == 0){
        return;
    }
    if (seconds > seconds_in_day){
        seconds %= seconds_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            if (seconds < seconds_in_minute){
                return;
            }
            _subtractMinutes(seconds % seconds_in_minute);
        }
        case tristan::time::Precision::SECONDS:{
            _subtractSeconds(seconds);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            _subtractMilliseconds(seconds * milliseconds_in_second);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _subtractMicroseconds(seconds * microseconds_in_second);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _subtractNanoseconds(seconds * nanoseconds_in_second);
            break;
        }
    }
}

void tristan::time::Time::subtractMilliseconds(uint64_t milliseconds){
    if (milliseconds == 0){
        return;
    }
    if (milliseconds > milliseconds_in_day){
        milliseconds %= milliseconds_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            if (milliseconds < milliseconds_in_minute){
                return;
            }
            _subtractMinutes(milliseconds % milliseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS:{
            if (milliseconds < milliseconds_in_second){
                return;
            }
            _subtractSeconds(milliseconds % milliseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            _subtractMilliseconds(milliseconds);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _subtractMicroseconds(milliseconds * microseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _subtractNanoseconds(milliseconds * nanoseconds_in_millisecond);
            break;
        }
    }
}

void tristan::time::Time::subtractMicroseconds(uint64_t microseconds){
    if (microseconds == 0){
        return;
    }
    if (microseconds > microseconds_in_day){
        microseconds %= microseconds_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            if (microseconds < microseconds_in_minute){
                return;
            }
            _subtractMinutes(microseconds % microseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS:{
            if (microseconds < microseconds_in_second){
                return;
            }
            _subtractSeconds(microseconds % microseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            if (microseconds < microseconds_in_millisecond){
                return;
            }
            _subtractMilliseconds(microseconds % microseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            _subtractMicroseconds(microseconds);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _subtractNanoseconds(microseconds * nanoseconds_in_microsecond);
            break;
        }
    }
}

void tristan::time::Time::subtractNanoseconds(uint64_t nanoseconds){
    if (nanoseconds == 0){
        return;
    }
    if (nanoseconds > nanoseconds_in_day){
        nanoseconds %= nanoseconds_in_day;
    }
    switch (m_precision){
        case tristan::time::Precision::MINUTES:{
            if (nanoseconds < nanoseconds_in_minute){
                return;
            }
            _subtractMinutes(nanoseconds % nanoseconds_in_minute);
        }
        case tristan::time::Precision::SECONDS:{
            if (nanoseconds < nanoseconds_in_second){
                return;
            }
            _subtractSeconds(nanoseconds % nanoseconds_in_second);
            break;
        }
        case tristan::time::Precision::MILLISECONDS:{
            if (nanoseconds < nanoseconds_in_millisecond){
                return;
            }
            _subtractMilliseconds(nanoseconds % nanoseconds_in_millisecond);
            break;
        }
        case tristan::time::Precision::MICROSECONDS:{
            if (nanoseconds < nanoseconds_in_microsecond){
                return;
            }
            _subtractMicroseconds(nanoseconds % nanoseconds_in_microsecond);
            break;
        }
        case tristan::time::Precision::NANOSECONDS:{
            _subtractNanoseconds(nanoseconds);
            break;
        }
    }
}

auto tristan::time::Time::hours() const -> uint8_t{
    return std::visit(
            [](const auto& value){
                return static_cast<uint8_t>(std::chrono::duration_cast<std::chrono::hours>(value).count());
            },
            this->m_time_since_day_start
    );
}

auto tristan::time::Time::minutes() const -> uint8_t{
    return std::visit(
            [](const auto& value){
                auto hours = std::chrono::duration_cast<std::chrono::hours>(value);
                auto minutes = std::chrono::duration_cast<std::chrono::minutes>(value) -
                               std::chrono::duration_cast<std::chrono::minutes>(hours);
                return static_cast<uint8_t>(minutes.count());
            },
            this->m_time_since_day_start
    );
}

auto tristan::time::Time::seconds() const -> uint8_t{
    if (m_precision < tristan::time::Precision::SECONDS){
        return 0;
    }

    return std::visit(
            [](const auto& value){
                auto minutes = std::chrono::duration_cast<std::chrono::minutes>(value);
                auto seconds = std::chrono::duration_cast<std::chrono::seconds>(value) -
                               std::chrono::duration_cast<std::chrono::seconds>(minutes);
                return static_cast<uint8_t>(seconds.count());
            },
            this->m_time_since_day_start
    );
}

auto tristan::time::Time::milliseconds() const -> uint16_t{
    if (m_precision < tristan::time::Precision::MILLISECONDS){
        return 0;
    }

    return std::visit(
            [](const auto& value){
                auto seconds = std::chrono::duration_cast<std::chrono::seconds>(value);
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(value) -
                                    std::chrono::duration_cast<std::chrono::milliseconds>(seconds);
                return static_cast<uint16_t>(milliseconds.count());
            },
            this->m_time_since_day_start
    );
}

auto tristan::time::Time::microseconds() const -> uint16_t{
    if (m_precision < tristan::time::Precision::MICROSECONDS){
        return 0;
    }

    return std::visit(
            [](const auto& value){
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(value);
                auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(value) -
                                    std::chrono::duration_cast<std::chrono::microseconds>(milliseconds);
                return static_cast<uint16_t>(microseconds.count());
            },
            this->m_time_since_day_start
    );
}

auto tristan::time::Time::nanoseconds() const -> uint16_t{
    if (m_precision < tristan::time::Precision::NANOSECONDS){
        return 0;
    }

    return std::visit(
            [](const auto& value){
                auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(value);
                auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(value) -
                                   std::chrono::duration_cast<std::chrono::nanoseconds>(microseconds);
                return static_cast<uint16_t>(nanoseconds.count());
            },
            this->m_time_since_day_start
    );
}

auto tristan::time::Time::localTime(Precision precision) -> tristan::time::Time{
    tristan::time::Time l_time(precision);

    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    tm local_time = *std::localtime(&time);
    tm utc_time = *std::gmtime(&time);
    auto hours = static_cast<int8_t>(local_time.tm_hour - utc_time.tm_hour);
    if (hours < -14) {
        hours += 24;
    }
    auto minutes = static_cast<int8_t>(local_time.tm_min - utc_time.tm_min);

    if (hours > 14 || hours < -14 || minutes > 59 || minutes < -59){
        throw std::range_error(
                "tristan::time::Time::localTime(Precision precision): Timezone value is out of possible range");
    }

    if (hours >= 0){
        l_time.addHours(hours);
    }
    else{
        l_time.subtractHours(std::abs(hours));
    }
    if (minutes >= 0){
        l_time.addMinutes(minutes);
    }
    else{
        l_time.subtractMinutes(std::abs(minutes));
    }

    l_time.m_offset = static_cast<tristan::time::TimeZone>(hours);

    return l_time;
}

std::string tristan::time::Time::toString(bool show_offset) const{
    std::string l_time;

    auto hours = this->hours();
    if (hours < 10){
        l_time += '0';
    }
    l_time += std::to_string(hours);
    l_time += ':';
    auto minutes = this->minutes();
    if (minutes < 10){
        l_time += '0';
    }
    l_time += std::to_string(minutes);

    if (m_precision >= tristan::time::Precision::SECONDS){
        l_time += ':';
        auto seconds = this->seconds();
        if (seconds < 10){
            l_time += '0';
        }
        l_time += std::to_string(seconds);
    }

    if (m_precision >= tristan::time::Precision::MILLISECONDS){
        l_time += '.';
        auto milliseconds = this->milliseconds();
        if (milliseconds < 100){
            l_time += '0';
        }
        if (milliseconds < 10){
            l_time += '0';
        }
        l_time += std::to_string(milliseconds);
    }

    if (m_precision >= tristan::time::Precision::MICROSECONDS){
        l_time += '.';
        auto microseconds = this->microseconds();
        if (microseconds < 100){
            l_time += '0';
        }
        if (microseconds < 10){
            l_time += '0';
        }
        l_time += std::to_string(microseconds);
    }

    if (m_precision == tristan::time::Precision::NANOSECONDS){
        l_time += '.';
        auto nanoseconds = this->nanoseconds();
        if (nanoseconds < 100){
            l_time += '0';
        }
        if (nanoseconds < 10){
            l_time += '0';
        }
        l_time += std::to_string(nanoseconds);
    }
    if (show_offset){
        if (m_offset >= tristan::time::TimeZone::UTC){
            l_time += '+';
            if (m_offset < tristan::time::TimeZone::EAST_10){
                l_time += '0';
            }
        }
        l_time += std::to_string(static_cast<int8_t>(m_offset));
        if (m_offset > tristan::time::TimeZone::WEST_10 && m_offset < tristan::time::TimeZone::UTC){
            l_time += '0';
        }
    }
    return l_time;
}

bool tristan::time::operator!=(const tristan::time::Time& l, const tristan::time::Time& r){
    return !(l == r);
}

bool tristan::time::operator>(const tristan::time::Time& l, const tristan::time::Time& r){
    return !(l <= r);
}

bool tristan::time::operator<=(const tristan::time::Time& l, const tristan::time::Time& r){
    return (l < r || l == r);
}

bool tristan::time::operator>=(const tristan::time::Time& l, const tristan::time::Time& r){
    return (l > r || l == r);
}

std::ostream& tristan::time::operator<<(std::ostream& out, const tristan::time::Time& Time){
    out << Time.toString(true);
    return out;
}

auto
tristan::time::operator+(const tristan::time::Time& l, const tristan::time::Time& r) -> tristan::time::Time{
    auto time = l;
    time.m_precision = std::max(l.precision(), r.precision());
    switch (time.m_precision){
        case tristan::time::Precision::NANOSECONDS:{
            time.addNanoseconds(r.nanoseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MICROSECONDS:{
            time.addMicroseconds(r.microseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MILLISECONDS:{
            time.addMilliseconds(r.milliseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::SECONDS:{
            time.addSeconds(r.seconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MINUTES:{
            time.addMinutes(r.minutes());
            time.addHours(r.hours());
            break;
        }
    }
    return time;
}

auto
tristan::time::operator-(const tristan::time::Time& l, const tristan::time::Time& r) -> tristan::time::Time{
    auto time = l;
    time.m_precision = std::max(l.precision(), r.precision());
    switch (time.m_precision){
        case tristan::time::Precision::NANOSECONDS:{
            time.subtractNanoseconds(r.nanoseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MICROSECONDS:{
            time.subtractMicroseconds(r.microseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MILLISECONDS:{
            time.subtractMilliseconds(r.milliseconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::SECONDS:{
            time.subtractSeconds(r.seconds());
            [[fallthrough]];
        }
        case tristan::time::Precision::MINUTES:{
            time.subtractMinutes(r.minutes());
            time.subtractHours(r.hours());
            break;
        }
    }
    return time;

}

void tristan::time::Time::_addMinutes(uint64_t minutes){
    auto& l_minutes = std::get<std::chrono::minutes>(m_time_since_day_start);
    l_minutes += std::chrono::minutes{minutes};
    if (l_minutes.count() > minutes_in_day){
        l_minutes %= minutes_in_day;
    }
}

void tristan::time::Time::_addSeconds(uint64_t seconds){
    auto& l_seconds = std::get<std::chrono::seconds>(m_time_since_day_start);
    l_seconds += std::chrono::seconds{seconds};
    if (l_seconds.count() > seconds_in_day){
        l_seconds %= seconds_in_day;
    }
}

void tristan::time::Time::_addMilliseconds(uint64_t milliseconds){
    auto& l_milliseconds = std::get<std::chrono::milliseconds>(m_time_since_day_start);
    l_milliseconds += std::chrono::milliseconds{milliseconds};
    if (l_milliseconds.count() > milliseconds_in_day){
        l_milliseconds %= milliseconds_in_day;
    }
}

void tristan::time::Time::_addMicroseconds(uint64_t microseconds){
    auto& l_microseconds = std::get<std::chrono::microseconds>(m_time_since_day_start);
    l_microseconds += std::chrono::microseconds{microseconds};
    if (l_microseconds.count() > microseconds_in_day){
        l_microseconds %= microseconds_in_day;
    }
}

void tristan::time::Time::_addNanoseconds(uint64_t nanoseconds){
    auto& l_nanoseconds = std::get<std::chrono::nanoseconds>(m_time_since_day_start);
    l_nanoseconds += std::chrono::nanoseconds{nanoseconds};
    if (l_nanoseconds.count() > nanoseconds_in_day){
        l_nanoseconds %= nanoseconds_in_day;
    }
}

void tristan::time::Time::_subtractMinutes(uint64_t minutes){
    auto& l_minutes = std::get<std::chrono::minutes>(m_time_since_day_start);
    l_minutes -= std::chrono::minutes{minutes};
    if (l_minutes.count() < 0){
        l_minutes += std::chrono::minutes{minutes_in_day};
    }
}

void tristan::time::Time::_subtractSeconds(uint64_t seconds){
    auto& l_seconds = std::get<std::chrono::seconds>(m_time_since_day_start);
    l_seconds -= std::chrono::seconds{seconds};
    if (l_seconds.count() < 0){
        l_seconds += std::chrono::seconds{seconds_in_day};
    }
}

void tristan::time::Time::_subtractMilliseconds(uint64_t milliseconds){
    auto& l_milliseconds = std::get<std::chrono::milliseconds>(m_time_since_day_start);
    l_milliseconds -= std::chrono::milliseconds{milliseconds};
    if (l_milliseconds.count() < 0){
        l_milliseconds += std::chrono::milliseconds{milliseconds_in_day};
    }
}

void tristan::time::Time::_subtractMicroseconds(uint64_t microseconds){
    auto& l_microseconds = std::get<std::chrono::microseconds>(m_time_since_day_start);
    l_microseconds -= std::chrono::microseconds{microseconds};
    if (l_microseconds.count() < 0){
        l_microseconds += std::chrono::microseconds{microseconds_in_day};
    }
}

void tristan::time::Time::_subtractNanoseconds(uint64_t nanoseconds){
    auto& l_nanoseconds = std::get<std::chrono::nanoseconds>(m_time_since_day_start);
    l_nanoseconds -= std::chrono::nanoseconds{nanoseconds};
    if (l_nanoseconds.count() < 0){
        l_nanoseconds += std::chrono::nanoseconds{nanoseconds_in_day};
    }
}

namespace{
    auto checkTimeFormat(const std::string& time) -> bool{

        auto length = time.length();
        if (length < 5 || length > 20){
            return false;
        }
        for (size_t i = 0; i < length; ++i){
            if ((time[i] < '0' || time[i] > '9') && (time[i] != ':' && time[i] != '.')){
                return false;
            }
            if (i < 2 && (time[i] < '0' || time[i] > '9')){
                return false;
            }
            if (i == 2 && time[i] != ':'){
                return false;
            }
            if ((i > 2 && i < 5) && (time[i] < '0' || time[i] > '9')){
                return false;
            }
            if (i == 5 && time[i] != ':'){
                return false;
            }
            if ((i > 5 && i < 8) && (time[i] < '0' || time[i] > '9')){
                return false;
            }
            if (i == 8 && time[i] != '.'){
                return false;
            }
            if ((i > 8 && i < 12) && (time[i] < '0' || time[i] > '9')){
                return false;
            }
            if (i == 12 && time[i] != '.'){
                return false;
            }
            if ((i > 12 && i < 16) && (time[i] < '0' || time[i] > '9')){
                return false;
            }
            if (i == 16 && time[i] != '.'){
                return false;
            }
            if ((i > 16 && i < 20) && (time[i] < '0' || time[i] > '9')){
                return false;
            }
        }
        return true;
    }
} //End of unnamed namespace