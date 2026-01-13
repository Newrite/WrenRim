export module WrenRim.Wren.Wrappers.HitData;

import WrenRim.Wren.Wrappers.Actor;
import WrenRim.Wren.Wrappers.Weapon;
import WrenRim.Core.Utility;

namespace wren::wrappers
{
  export class hit_data
  {
  public:
    hit_data(RE::HitData* data)
    {
      if (data) {
        data_ = data;
      }
    }

    hit_data() = default;

    [[nodiscard]] RE::HitData* get() const
    {
      return data_;
    }

    auto is_valid() const -> bool
    {
      return data_ ? true : false;
    }

    auto get_attacker() const -> actor
    {
      if (!is_valid()) return actor();
      
      if (auto attacker = core::utility::get_actor_from_actor_handle(data_->aggressor)) {
        return actor{attacker};
      }
      
      return actor();
    }
    
    auto get_target() const -> actor
    {
      if (!is_valid()) return actor();
      
      if (auto target = core::utility::get_actor_from_actor_handle(data_->target)) {
        return actor{target};
      }
      
      return actor();
    }
    
    auto get_total_damage() const -> float
    {
      if (!is_valid()) return 0.f;
      
      return data_->totalDamage;
    }
    
    auto set_total_damage(const float value) const -> void
    {
      if (!is_valid()) return;
      
      data_->totalDamage = value;
    }
    
    auto get_weapon() const -> weapon
    {
      if (!is_valid() || !data_->weapon) return weapon();
      return weapon{data_->weapon};
    }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<hit_data>("HitData");
      cls.ctor<>();
      cls.func<&hit_data::is_valid>("isValid");
      cls.func<&hit_data::get_attacker>("getAttacker");
      cls.func<&hit_data::get_target>("getTarget");
      cls.func<&hit_data::get_total_damage>("getTotalDamage");
      cls.func<&hit_data::set_total_damage>("setTotalDamage");
      cls.func<&hit_data::get_weapon>("getWeapon");
    }

  private:
    RE::HitData* data_{nullptr};
  };
}
