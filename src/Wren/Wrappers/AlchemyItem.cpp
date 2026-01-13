export module WrenRim.Wren.Wrappers.AlchemyItem;

namespace wren::wrappers
{
  export class alchemy_item
  {
  public:
    alchemy_item(RE::AlchemyItem* p)
    {
      if (p) {
        form_id_ = p->GetFormID();
      }
    }
    
    alchemy_item(const RE::FormID form_id)
    {
      if (auto p = RE::TESForm::LookupByID<RE::AlchemyItem>(form_id)) {
        item_ = p;
        form_id_ = form_id;
      }
    }

    alchemy_item() = default;
    
    auto is_valid() const -> bool
    {
      const auto potion = get();
      return static_cast<bool>(potion);
    }

    [[nodiscard]] RE::AlchemyItem* get() const
    {
      if (item_) return item_;
      return RE::TESForm::LookupByID<RE::AlchemyItem>(form_id_);
    }

    bool is_poison() const
    {
      auto ptr = get();
      return ptr ? ptr->IsPoison() : false;
    }

    bool is_food() const
    {
      auto ptr = get();
      return ptr ? ptr->IsFood() : false;
    }

    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
    }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<alchemy_item>("AlchemyItem");
      cls.ctor<>();
      cls.func<&alchemy_item::is_valid>("isValid");
      cls.func<&alchemy_item::is_poison>("isPoison");
      cls.func<&alchemy_item::is_food>("isFood");
      cls.func<&alchemy_item::get_name>("getName");
    }

  private:
    RE::AlchemyItem* item_{nullptr};
    RE::FormID form_id_{0};
  };
}
