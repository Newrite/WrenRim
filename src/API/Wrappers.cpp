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
        
        actor() = default;

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
            cls.ctor<>();
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
        
        potion() = default;

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
            cls.ctor<>();
            cls.func<&potion::is_poison>("isPoison");
            cls.func<&potion::is_food>("isFood");
            cls.func<&potion::get_name>("getName");
        }

    private:
        RE::FormID form_id_{ 0 };
    };
    
    export void bind_wrappers(wrenbind17::VM& vm) {
        // Bind Actor to "Skyrim/Actor"
        auto& mActor = vm.module("Skyrim/Actor");
        actor::bind(mActor);

        // Bind Potion to "Skyrim/Potion"
        auto& mPotion = vm.module("Skyrim/Potion");
        potion::bind(mPotion);
    }
}
