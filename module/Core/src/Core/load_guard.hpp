#ifndef GAMEENGINEBONGO_LOAD_GUARD_HPP
#define GAMEENGINEBONGO_LOAD_GUARD_HPP

template<typename T>
class load_guard
{
public:
    load_guard(const load_guard &) = delete;
    load_guard &operator=(const load_guard &) = delete;

    constexpr load_guard(T &target)
        : m_target{ target }
    {
        m_target.load();
    }
    ~load_guard() noexcept(noexcept(m_target.load())) { m_target.unload(); }

private:
    T &m_target;
};

#endif// GAMEENGINEBONGO_LOAD_GUARD_HPP
