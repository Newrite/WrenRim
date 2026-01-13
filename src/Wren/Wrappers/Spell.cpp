export module WrenRim.Wren.Wrappers.Spell;

import WrenRim.Wren.Wrappers.Actor;

namespace wren::wrappers
{
  export class spell
  {
  public:
    spell(RE::SpellItem* spell)
    {
      if (spell) {
        spell_ = spell;
        form_id_ = spell->GetFormID();
      }
    }

    spell(const RE::FormID form_id)
    {
      if (auto sp = RE::TESForm::LookupByID<RE::SpellItem>(form_id)) {
        spell_ = sp;
        form_id_ = form_id;
      }
    }

    spell() = default;

    [[nodiscard]] RE::SpellItem* get() const
    {
      if (spell_) return spell_;
      return RE::TESForm::LookupByID<RE::SpellItem>(form_id_);
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

    float get_cast_time() const
    {
      auto ptr = get();
      return ptr ? ptr->GetChargeTime() : 0.0f;
    }

    int32_t get_magicka_cost() const
    {
      auto ptr = get();
      return ptr ? ptr->data.costOverride : 0;
    }

    float get_effective_magicka_cost(const actor& caster) const
    {
      auto ptr = get();
      auto caster_ptr = caster.get();
      return (ptr && caster_ptr) ? ptr->CalculateMagickaCost(caster_ptr) : 0.f;
    }

    // Spell Type: 0=Spell, 1=Disease, 2=Power, 3=LesserPower, 4=Ability, 5=Poison, 6=Addition, 7=Voice
    int32_t get_spell_type() const
    {
      auto ptr = get();
      return ptr ? static_cast<int32_t>(ptr->GetSpellType()) : -1;
    }

    // Casting Type: 0=ConstantEffect, 1=FireAndForget, 2=Concentration, 3=Scroll
    int32_t get_casting_type() const
    {
      auto ptr = get();
      return ptr ? static_cast<int32_t>(ptr->GetCastingType()) : -1;
    }

    // Delivery: 0=Self, 1=Touch, 2=Aimed, 3=TargetActor, 4=TargetLocation
    int32_t get_delivery() const
    {
      auto ptr = get();
      return ptr ? static_cast<int32_t>(ptr->GetDelivery()) : -1;
    }

    bool is_hostile() const
    {
      auto ptr = get();
      return ptr ? ptr->IsHostile() : false;
    }

    void set_cast_time(float time)
    {
        auto ptr = get();
        if (ptr) ptr->data.chargeTime = time;
    }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<spell>("Spell");
      cls.ctor<>();
      cls.func<&spell::is_valid>("isValid");
      cls.func<&spell::get_name>("getName");
      cls.func<&spell::get_cast_time>("getCastTime");
      cls.func<&spell::get_magicka_cost>("getMagickaCost");
      cls.func<&spell::get_effective_magicka_cost>("getEffectiveMagickaCost");
      cls.func<&spell::get_spell_type>("getSpellType");
      cls.func<&spell::get_casting_type>("getCastingType");
      cls.func<&spell::get_delivery>("getDelivery");
      cls.func<&spell::is_hostile>("isHostile");
      cls.func<&spell::set_cast_time>("setCastTime");
    }

  private:
    RE::SpellItem* spell_{nullptr};
    RE::FormID form_id_{0};
  };
}
