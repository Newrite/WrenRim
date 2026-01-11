export module WrenRim.API.Wrappers;

namespace api::wrappers
{
    export class actor {
    public:
        actor(RE::Actor* a) {
            if (a) {
                handle_ = a->GetHandle();
            }
        }
        
        [[nodiscard]] bool is_valid() const {
            return static_cast<bool>(handle_);
        }

        [[nodiscard]] RE::Actor* get() const {
            return handle_.get().get();
        }

        std::string get_name() const {
            auto ptr = get();
            return ptr ? ptr->GetName() : "";
        }

        static void bind(wrenbind17::ForeignModule& module) {
            auto& cls = module.klass<actor>("Actor");
            cls.func<&actor::get_name>("getName");
        }

    private:
        RE::ActorHandle handle_;
    };

    export class potion {
    public:
        potion(RE::AlchemyItem* p) {
            if (p) {
                form_id_ = p->GetFormID();
            }
        }

        [[nodiscard]] RE::AlchemyItem* get() const {
            return RE::TESForm::LookupByID<RE::AlchemyItem>(form_id_);
        }

        bool is_poison() const {
            auto ptr = get();
            return ptr ? ptr->IsPoison() : false;
        }
        
        bool is_food() const {
             auto ptr = get();
             return ptr ? ptr->IsFood() : false;
        }

        std::string get_name() const {
            auto ptr = get();
            return ptr ? ptr->GetName() : "";
        }

        static void bind(wrenbind17::ForeignModule& module) {
            auto& cls = module.klass<potion>("Potion");
            cls.func<&potion::is_poison>("isPoison");
            cls.func<&potion::is_food>("isFood");
            cls.func<&potion::get_name>("getName");
        }

    private:
        RE::FormID form_id_{ 0 };
    };
    
    export void bind_wrappers(wrenbind17::VM& vm) {
        auto& module = vm.module("WrenRimStd/Skyrim");
        actor::bind(module);
        potion::bind(module);
    }
}
