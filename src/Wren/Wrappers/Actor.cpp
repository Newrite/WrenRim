export module WrenRim.Wren.Wrappers.Actor;

namespace wren::wrappers
{
  export class actor
  {
  public:
    actor(RE::Actor* a)
    {
      if (a) {
        handle_ = a->GetHandle();
      }
    }

    actor(const RE::ActorHandle& handle)
    {
      if (handle) {
        handle_ = handle;
      }
    }

    actor() = default;

    [[nodiscard]] RE::Actor* get() const
    {
      return handle_.get().get();
    }

    [[nodiscard]] bool is_valid() const
    {
      return static_cast<bool>(handle_);
    }

    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
    }

    auto get_actor_value_by_enum(const uint32_t av_raw) const -> float
    {
      if (!is_valid()) return 0.f;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return 0.f;
      
      const auto av = static_cast<RE::ActorValue>(av_raw);

      return owner->GetActorValue(av);
    }

    auto get_actor_value_by_string(const std::string& str_av) const -> float
    {
      if (!is_valid()) return 0.f;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return 0.f;

      const auto av = RE::ActorValueList::LookupActorValueByName(str_av.c_str());

      if (av == RE::ActorValue::kNone) return 0.f;

      return owner->GetActorValue(av);
    }
    
    auto mod_actor_value_by_enum(const uint32_t av_raw, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;
      
      const auto av = static_cast<RE::ActorValue>(av_raw);

      return owner->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, value);
    }

    auto mod_actor_value_by_string(const std::string& str_av, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;
      
      const auto av = RE::ActorValueList::LookupActorValueByName(str_av.c_str());

      return owner->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, value);
    }
    
    auto restore_actor_value_by_enum(const uint32_t av_raw, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;
      
      const auto av = static_cast<RE::ActorValue>(av_raw);

      return owner->RestoreActorValue(av, value);
    }

    auto restore_actor_value_by_string(const std::string& str_av, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;
      
      const auto av = RE::ActorValueList::LookupActorValueByName(str_av.c_str());

      return owner->RestoreActorValue(av, value);
    }
    
    auto damage_actor_value_by_enum(const uint32_t av_raw, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;
      
      const auto av = static_cast<RE::ActorValue>(av_raw);

      return owner->DamageActorValue(av, value);
    }

    auto damage_actor_value_by_string(const std::string& str_av, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;
      
      const auto av = RE::ActorValueList::LookupActorValueByName(str_av.c_str());

      return owner->DamageActorValue(av, value);
    }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<actor>("Actor");
      cls.ctor<>();
      cls.func<&actor::get_name>("getName");
      cls.func<&actor::is_valid>("isValid");
      cls.func<&actor::get_actor_value_by_enum>("getActorValueByEnum");
      cls.func<&actor::get_actor_value_by_string>("getActorValueByString");
      cls.func<&actor::mod_actor_value_by_enum>("modActorValueByEnum");
      cls.func<&actor::mod_actor_value_by_string>("modActorValueByString");
      cls.func<&actor::restore_actor_value_by_enum>("restoreActorValueByEnum");
      cls.func<&actor::restore_actor_value_by_string>("restoreActorValueByString");
      cls.func<&actor::damage_actor_value_by_enum>("damageActorValueByEnum");
      cls.func<&actor::damage_actor_value_by_string>("damageActorValueByString");
    }

  private:
    RE::ActorHandle handle_;
  };
}
