#ifndef TIME_HPP
#define TIME_HPP

#include <string>
#include <iostream>
#include <chrono>
#include <variant>

/// \brief Namespace which includes time handlers
namespace tristan::time{

    /// \brief Enum which represents precisions used in implementation.
    enum class Precision : uint8_t{
        MINUTES,
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS
    };
    /// \brief enum which represents possible time zone offsets.
    enum class TimeZone : int8_t{
        WEST_12 [[maybe_unused]] = -12,
        WEST_11 [[maybe_unused]] = -11,
        WEST_10 [[maybe_unused]] = -10,
        WEST_9 [[maybe_unused]] = -9,
        WEST_8 [[maybe_unused]] = -8,
        WEST_7 [[maybe_unused]] = -7,
        WEST_6 [[maybe_unused]] = -6,
        WEST_5 [[maybe_unused]] = -5,
        WEST_4 [[maybe_unused]] = -4,
        WEST_3 [[maybe_unused]] = -3,
        WEST_2 [[maybe_unused]] = -2,
        WEST_1 [[maybe_unused]] = -1,
        UTC = 0,
        EAST_1 [[maybe_unused]] = 1,
        EAST_2 [[maybe_unused]] = 2,
        EAST_3 [[maybe_unused]] = 3,
        EAST_4 [[maybe_unused]] = 4,
        EAST_5 [[maybe_unused]] = 5,
        EAST_6 [[maybe_unused]] = 6,
        EAST_7 [[maybe_unused]] = 7,
        EAST_8 [[maybe_unused]] = 8,
        EAST_9 [[maybe_unused]] = 9,
        EAST_10 [[maybe_unused]] = 10,
        EAST_11 [[maybe_unused]] = 11,
        EAST_12 [[maybe_unused]] = 12,
    };

    /// \brief Class to handle time
    /// \headerfile time.hpp
    class Time{
        friend auto operator+(const Time& l, const Time& r) -> Time;
        friend auto operator-(const Time& l, const Time& r) -> Time;

    public:
        /// \brief Creates Time object from system clock. UTC time zone is set.
        /// \param precision Precision.
        explicit Time(Precision precision = Precision::SECONDS);
        /// \brief Minutes precision constructor.
        /// \param hours uint8_t.
        /// \param minutes uint8_t.
        /// \throws std::range_error.
        explicit Time(uint8_t hours, uint8_t minutes);
        /// \brief Seconds precision constructor. UTC time zone is set.
        /// \param hours uint8_t.
        /// \param minutes uint8_t.
        /// \param seconds uint8_t.
        /// \throws std::range_error.
        explicit Time(uint8_t hours, uint8_t minutes, uint8_t seconds);
        /// \brief Milliseconds precision constructor. UTC time zone is set.
        /// \param hours uint8_t.
        /// \param minutes uint8_t.
        /// \param seconds uint8_t.
        /// \param milliseconds uint16_t.
        /// \throws std::range_error.
        explicit Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
        /// \brief Microseconds precision constructor. UTC time zone is set.
        /// \param hours uint8_t.
        /// \param minutes uint8_t.
        /// \param seconds uint8_t.
        /// \param milliseconds uint16_t.
        /// \param microseconds uint16_t.
        /// \throws std::range_error.
        explicit Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds);
        /// \brief Nanoseconds precision constructor. UTC time zone is set.
        /// \param hours uint8_t.
        /// \param minutes uint8_t.
        /// \param seconds uint8_t.
        /// \param milliseconds uint16_t.
        /// \param microseconds uint16_t.
        /// \param nanoseconds uint16_t.
        /// \throws std::range_error.
        explicit Time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, uint16_t microseconds,
                      uint16_t nanoseconds);
        /// \brief Parses the string provided and create time object.
        /// \param time std::string representing time in following <b>formats</b>:
        /// \li [HH:MM] - Minutes precision.
        /// \li [HH:MM+(-)HH] - Minutes precision with offset.
        /// \li [HH:MM:SS] - Seconds precision.
        /// \li [HH:MM:SS+(-)HH] - Seconds precision with offset.
        /// \li [HH:MM:SS.mmm] - Milliseconds precision.
        /// \li [HH:MM:SS.mmm+(-)HH] - Milliseconds precision with offset.
        /// \li [HH:MM:SS.mmm.mmm] - Microseconds precision.
        /// \li [HH:MM:SS.mmm.mmm+(-)HH] - Microseconds precision with offset.
        /// \li [HH:MM:SS.mmm.mmm.nnn] - Nanoseconds precision.
        /// \li [HH:MM:SS.mmm.mmm.nnn+(-)HH] - Nanoseconds precision with offset.
        /// \throws std::invali_argument, std::range_error.
        explicit Time(const std::string& time);
        /// \brief Copy constructor
        Time(const Time&) = default;
        /// \brief Move constructor
        Time(Time&&) = default;
        /// \brief Copy assignment operator
        auto operator=(const Time&) -> Time& = default;
        /// \brief Move assignment operator
        auto operator=(Time&&) -> Time& = default;
        /// \brief Operator equal.
        /// \note Precision is taken into account. That is if comparable objects are having different precision - false is returned.
        auto operator==(const Time& r) const -> bool;
        /// \brief Operator less then.
        /// \note Precision is taken into account. That is if comparable objects are having different precision - false is returned.
        auto operator<(const Time& r) const -> bool;
        /// \brief Unary plus operator.
        void operator+=(const Time& r);
        /// \brief Unary minus operator.
        void operator-=(const Time& r);
        /// \brief Destructor
        virtual ~Time() = default;

        /// \brief Sets timezone offset. Only ISO hour based offsets are considered.
        /// \param offset TimeZone.
        [[maybe_unused]] void setOffset(TimeZone offset){
            m_offset = offset;
        }

        /// \brief Adds hours to Time object. This is a convenience function which converts hours to minutes and invokes addMinutes(uint64_t minutes) function.
        /// \param hours uint64_t.
        void addHours(uint64_t hours);
        /// \brief Adds minutes to Time object.
        /// \param minutes uint64_t.
        void addMinutes(uint64_t minutes);
        /// \brief Adds seconds to Time object.
        /// \note Precision is taken into account. That is in case of MINUTES precision if number of seconds is less then 1 minute operation is meaningless and will not have any effect.
        /// \param seconds uint64_t.
        void addSeconds(uint64_t seconds);
        /// \brief Adds milliseconds to Time object.
        /// \note Precision is taken into account. That is in case of SECONDS or MINUTES precision if number of milliseconds is less then 1 second operation is meaningless and will not have any effect.
        /// \param milliseconds uint64_t.
        void addMilliseconds(uint64_t milliseconds);
        /// \brief Adds microseconds to Time object.
        /// \note Precision is taken into account. That is in case of SECONDS, MINUTES or MILLISECONDS precision if number of microseconds is less then 1 millisecond operation is meaningless and will not have any effect.
        /// \param microseconds uint64_t.
        void addMicroseconds(uint64_t microseconds);
        /// \brief Adds nanoseconds to Time object.
        /// \note Precision is taken into account. That is in case of SECONDS, MINUTES, MILLISECONDS or MICROSECONDS precision if number of nanosecond is less then 1 microsecond operation is meaningless and will not have any effect.
        /// \param nanoseconds uint64_t.
        void addNanoseconds(uint64_t nanoseconds);
        /// \brief subtracts hours from Time object. This is a convenience function which converts hours to minutes and invokes subtractMinutes(uint64_t minutes) function
        /// \param hours uint64_t.
        void subtractHours(uint64_t hours);
        /// \brief subtracts minutes from Time object.
        /// \param minutes uint64_t.
        void subtractMinutes(uint64_t minutes);
        /// \brief subtracts seconds from Time object.
        /// \note Precision is taken into account. That is in case of MINUTES precision if number of seconds is less then 1 minute operation is meaningless and will not have any effect.
        /// \param seconds uint64_t.
        void subtractSeconds(uint64_t seconds);
        /// \brief subtracts milliseconds from Time object.
        /// \note Precision is taken into account. That is in case of SECONDS or MINUTES precision if number of milliseconds is less then 1 second operation is meaningless and will not have any effect.
        /// \param milliseconds uint64_t.
        void subtractMilliseconds(uint64_t milliseconds);
        /// \brief subtracts microseconds from Time object.
        /// \note Precision is taken into account. That is in case of SECONDS, MINUTES, MILLISECONDS or MICROSECONDS precision if number of nanosecond is less then 1 microsecond operation is meaningless and will not have any effect.
        /// \param microseconds uint64_t.
        void subtractMicroseconds(uint64_t microseconds);
        /// \brief subtracts nanoseconds from Time object.
        /// \note Precision is taken into account. That is in case of SECONDS, MINUTES, MILLISECONDS or MICROSECONDS precision if number of nanosecond is less then 1 microsecond operation is meaningless and will not have any effect.
        /// \param nanoseconds uint64_t.
        void subtractNanoseconds(uint64_t nanoseconds);

        /// \brief Returns number of hours passed since day start.
        /// \return uint8_t.
        [[nodiscard]] auto hours() const -> uint8_t;
        /// \brief Returns number of minutes passed since hour start.
        /// \return uint8_t.
        [[nodiscard]] auto minutes() const -> uint8_t;
        /// \brief Returns number of seconds passed since minute start.
        /// \return uint8_t.
        [[nodiscard]] auto seconds() const -> uint8_t;
        /// \brief Returns number of milliseconds passed since second start.
        /// \return uint16_t.
        [[nodiscard]] auto milliseconds() const -> uint16_t;
        /// \brief Returns number of microseconds passed since millisecond start.
        /// \return uint16_t.
        [[nodiscard]] auto microseconds() const -> uint16_t;
        /// \brief Returns number of nanoseconds passed since microsecond start.
        /// \return uint16_t.
        [[nodiscard]] auto nanoseconds() const -> uint16_t;

        /// \brief Returns precision of Time object.
        /// \return Precision.
        [[nodiscard]] auto precision() const -> Precision{ return m_precision; }

        /**
         * \brief Returns current offset
         * \return
         */
        [[nodiscard]] auto offset() const -> TimeZone{ return m_offset; }

        /// \brief Creates Time object which represents localtime.
        /// \param precision Precision::SECONDS.
        /// \return Time.
        [[nodiscard]] static auto localTime(Precision precision = Precision::SECONDS) -> Time;
        /// \brief Generates string representation of time. By default return ISO standard representation in formats represented below.
        /// If show_offset is set to true each format will be suffixed by offset in form of +(-)hh.
        /// \return std::string to represent the time.
        /// \par Formats:
        /// \li [hours:minutes] - Minutes precision.
        /// \li [hours:minutes:seconds] - Seconds precision.
        /// \li [hours:minutes:seconds.milliseconds] - Milliseconds precision.
        /// \li [hours:minutes:seconds.milliseconds.microseconds] - Microseconds precision.
        /// \li [hours:minutes:seconds.milliseconds.microseconds.nanoseconds] - Nanoseconds precision.
        [[nodiscard]] virtual auto toString(bool show_offset) const -> std::string;

    protected:

    private:
        std::variant<std::chrono::minutes, std::chrono::seconds, std::chrono::milliseconds, std::chrono::microseconds, std::chrono::nanoseconds> m_time_since_day_start;

        TimeZone m_offset;

        Precision m_precision;

        void _addMinutes(uint64_t minutes);
        void _addSeconds(uint64_t seconds);
        void _addMilliseconds(uint64_t milliseconds);
        void _addMicroseconds(uint64_t microseconds);
        void _addNanoseconds(uint64_t nanoseconds);
        void _subtractMinutes(uint64_t minutes);
        void _subtractSeconds(uint64_t seconds);
        void _subtractMilliseconds(uint64_t milliseconds);
        void _subtractMicroseconds(uint64_t microseconds);
        void _subtractNanoseconds(uint64_t nanoseconds);
    };

    /// \brief Operator not equal to.
    auto operator!=(const Time& l, const Time& r) -> bool;
    /// \brief Operator greater than.
    auto operator>(const Time& l, const Time& r) -> bool;
    /// \brief Operator less than or equal to.
    auto operator<=(const Time& l, const Time& r) -> bool;
    /// \brief Operator greater than or equal to.
    auto operator>=(const Time& l, const Time& r) -> bool;

    /// \brief Addition operator.
    auto operator+(const Time& l, const Time& r) -> Time;
    /// \brief Subtraction operator.
    auto operator-(const Time& l, const Time& r) -> Time;
    /// \brief std::ostream operator.  Date::toString() is used.
    auto operator<<(std::ostream& out, const Time& time) -> std::ostream&;
}//namespace tristan::time

#endif // TIME_HPP
