export module WrenRim.Wren.Wrappers.Armor;

namespace wren::wrappers
{
  export class armor
  {
  public:
    armor(RE::TESObjectARMO* armo)
    {
      if (armo) {
        armor_ = armo;
        form_id_ = armo->GetFormID();
      }
    }

    armor(const RE::FormID form_id)
    {
      if (auto armo = RE::TESForm::LookupByID<RE::TESObjectARMO>(form_id)) {
        armor_ = armo;
        form_id_ = form_id;
      }
    }

    armor() = default;

    [[nodiscard]] RE::TESObjectARMO* get() const
    {
      if (armor_) return armor_;
      return RE::TESForm::LookupByID<RE::TESObjectARMO>(form_id_);
    }

    [[nodiscard]] bool is_valid() const
    {
      return static_cast<bool>(get());
    }

    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
    }

    float get_weight() const
    {
      auto ptr = get();
      return ptr ? ptr->GetWeight() : 0.0f;
    }

    int32_t get_value() const
    {
      auto ptr = get();
      return ptr ? ptr->GetGoldValue() : 0;
    }

    float get_armor_rating() const
    {
      auto ptr = get();
      return ptr ? ptr->GetArmorRating() : 0.0f;
    }

    // Returns: 0 = Light, 1 = Heavy, 2 = Clothing
    int32_t get_armor_type() const
    {
      auto ptr = get();
      if (!ptr) return -1;
      return static_cast<int32_t>(ptr->GetArmorType());
    }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<armor>("Armor");
      cls.ctor<>();
      cls.func<&armor::is_valid>("isValid");
      cls.func<&armor::get_name>("getName");
      cls.func<&armor::get_weight>("getWeight");
      cls.func<&armor::get_value>("getValue");
      cls.func<&armor::get_armor_rating>("getArmorRating");
      cls.func<&armor::get_armor_type>("getArmorType");
    }

  private:
    RE::TESObjectARMO* armor_{nullptr};
    RE::FormID form_id_{0};
  };
}