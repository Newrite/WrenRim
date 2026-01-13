export module WrenRim.Wren.Wrappers.Weapon;

namespace wren::wrappers
{
  export class weapon
  {
  public:
    weapon(RE::TESObjectWEAP* w)
    {
      if (w) {
        weapon_ = w;
        form_id_ = w->GetFormID();
      }
    }
    
    weapon(const RE::FormID form_id)
    {
      if (auto w = RE::TESForm::LookupByID<RE::TESObjectWEAP>(form_id)) {
        weapon_ = w;
        form_id_ = form_id;
      }
    }

    weapon() = default;

    [[nodiscard]] RE::TESObjectWEAP* get() const
    {
      if (weapon_) return weapon_;
      return RE::TESForm::LookupByID<RE::TESObjectWEAP>(form_id_);
    }

    [[nodiscard]] bool is_valid() const
    {
      return static_cast<bool>(weapon_);
    }

    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
    }

    float get_attack_damage() const
    {
      auto ptr = get();
      return ptr ? ptr->GetAttackDamage() : 0.0f;
    }

    float get_reach() const
    {
      auto ptr = get();
      return ptr ? ptr->GetReach() : 0.0f;
    }

    float get_speed() const
    {
      auto ptr = get();
      return ptr ? ptr->GetSpeed() : 0.0f;
    }

    float get_weight() const
    {
      auto ptr = get();
      return ptr ? ptr->GetWeight() : 0.0f;
    }

    float get_value() const
    {
      auto ptr = get();
      return ptr ? static_cast<float>(ptr->GetGoldValue()) : 0.0f;
    }
    
    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<weapon>("Weapon");
      cls.ctor<>();
      cls.func<&weapon::get_name>("getName");
      cls.func<&weapon::is_valid>("isValid");
      cls.func<&weapon::get_attack_damage>("getAttackDamage");
      cls.func<&weapon::get_reach>("getReach");
      cls.func<&weapon::get_speed>("getSpeed");
      cls.func<&weapon::get_weight>("getWeight");
      cls.func<&weapon::get_value>("getValue");
    }

  private:
    RE::TESObjectWEAP* weapon_{nullptr};
    RE::FormID form_id_{0};
  };
}
