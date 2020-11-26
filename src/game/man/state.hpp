#pragma once
#include <stack>
#include <memory>
#include <type_traits>

struct StateBase_t {
    virtual void update()  = 0;
    virtual bool alive()   = 0;
    virtual ~StateBase_t() = default;
};
struct StateManager_t {
    explicit StateManager_t() = default;

    template <typename State_t, typename... Ts>
    void pushState(Ts&&... args) {
        //Falla en la compilación si State_t no es del tipo StateBase_t
        static_assert(std::is_base_of<StateBase_t, State_t>::value, "ERROR! Invalid State_t");
        m_States.push( std::make_unique<State_t>(std::forward<decltype(args)>(args)...) );
    }

    void update() {
        if( alive() ) {
            auto& state { m_States.top() };
            if(state && state->alive()) //state != null
                state->update();
            else
                m_States.pop();
        }
    }

    bool alive() { return !m_States.empty(); }
private:
    std::stack<std::unique_ptr<StateBase_t>> m_States;
};
