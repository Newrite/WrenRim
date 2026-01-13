export module WrenRim.Wren.Wrappers.GFxValue;

namespace wren::wrappers
{
  export class gfx_value
  {
  public:
    gfx_value() : value_() {}
    gfx_value(const RE::GFxValue& val) : value_(val) {}
    gfx_value(double val) : value_(val) {}
    gfx_value(bool val) : value_(val) {}
    gfx_value(const std::string& val) : value_(val) {}

    // Copy/Move
    gfx_value(const gfx_value& other) : value_(other.value_) {}
    gfx_value(gfx_value&& other) noexcept : value_(std::move(other.value_)) {}
    gfx_value& operator=(const gfx_value& other) { value_ = other.value_; return *this; }
    gfx_value& operator=(gfx_value&& other) noexcept { value_ = std::move(other.value_); return *this; }

    [[nodiscard]] const RE::GFxValue& get() const { return value_; }
    [[nodiscard]] RE::GFxValue& get() { return value_; }

    // Factory methods to avoid constructor ambiguity in Wren
    static gfx_value from_bool(bool val) { return gfx_value(val); }
    static gfx_value from_number(double val) { return gfx_value(val); }
    static gfx_value from_string(const std::string& val) { return gfx_value(val); }

    // Type checks
    bool is_undefined() const { return value_.IsUndefined(); }
    bool is_null() const { return value_.IsNull(); }
    bool is_bool() const { return value_.IsBool(); }
    bool is_number() const { return value_.IsNumber(); }
    bool is_string() const { return value_.IsString(); }
    bool is_object() const { return value_.IsObject(); }
    bool is_array() const { return value_.IsArray(); }
    bool is_display_object() const { return value_.IsDisplayObject(); }

    // Getters
    bool get_bool() const { return value_.GetBool(); }
    double get_number() const { return value_.GetNumber(); }
    std::string get_string() const { return value_.GetString(); }

    // Setters
    void set_bool(bool val) { value_ = val; }
    void set_number(double val) { value_ = val; }
    void set_string(const std::string& val) { value_ = val; }
    void set_undefined() { value_.SetUndefined(); }
    void set_null() { value_.SetNull(); }

    // Object/Array operations
    bool has_member(const std::string& name) const { return value_.HasMember(name.c_str()); }
    
    gfx_value get_member(const std::string& name) const {
        RE::GFxValue result;
        value_.GetMember(name.c_str(), &result);
        return gfx_value(result);
    }

    void set_member(const std::string& name, const gfx_value& val) {
        value_.SetMember(name.c_str(), val.get());
    }

    void invoke(const std::string& name, const std::vector<gfx_value>& args) {
        std::vector<RE::GFxValue> gfx_args;
        gfx_args.reserve(args.size());
        for (const auto& arg : args) gfx_args.push_back(arg.get());
        value_.Invoke(name.c_str(), nullptr, gfx_args.data(), gfx_args.size());
    }

    gfx_value invoke_result(const std::string& name, const std::vector<gfx_value>& args) {
        std::vector<RE::GFxValue> gfx_args;
        gfx_args.reserve(args.size());
        for (const auto& arg : args) gfx_args.push_back(arg.get());
        RE::GFxValue result;
        value_.Invoke(name.c_str(), &result, gfx_args.data(), gfx_args.size());
        return gfx_value(result);
    }

    uint32_t get_array_size() const { return value_.GetArraySize(); }
    
    void set_array_size(uint32_t size) { value_.SetArraySize(size); }

    gfx_value get_element(uint32_t index) const {
        RE::GFxValue result;
        value_.GetElement(index, &result);
        return gfx_value(result);
    }

    void set_element(uint32_t index, const gfx_value& val) {
        value_.SetElement(index, val.get());
    }

    void push_back(const gfx_value& val) {
        value_.PushBack(val.get());
    }

    // Text
    void set_text(const std::string& text) { value_.SetText(text.c_str()); }
    void set_text_html(const std::string& html) { value_.SetTextHTML(html.c_str()); }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<gfx_value>("GFxValue");
      cls.ctor<>();
      
      // Use funcStatic for static methods
      cls.funcStatic<&gfx_value::from_bool>("fromBool");
      cls.funcStatic<&gfx_value::from_number>("fromNumber");
      cls.funcStatic<&gfx_value::from_string>("fromString");

      cls.func<&gfx_value::is_undefined>("isUndefined");
      cls.func<&gfx_value::is_null>("isNull");
      cls.func<&gfx_value::is_bool>("isBool");
      cls.func<&gfx_value::is_number>("isNumber");
      cls.func<&gfx_value::is_string>("isString");
      cls.func<&gfx_value::is_object>("isObject");
      cls.func<&gfx_value::is_array>("isArray");
      cls.func<&gfx_value::is_display_object>("isDisplayObject");

      cls.func<&gfx_value::get_bool>("getBool");
      cls.func<&gfx_value::get_number>("getNumber");
      cls.func<&gfx_value::get_string>("getString");

      cls.func<&gfx_value::set_bool>("setBool");
      cls.func<&gfx_value::set_number>("setNumber");
      cls.func<&gfx_value::set_string>("setString");
      cls.func<&gfx_value::set_undefined>("setUndefined");
      cls.func<&gfx_value::set_null>("setNull");

      cls.func<&gfx_value::has_member>("hasMember");
      cls.func<&gfx_value::get_member>("getMember");
      cls.func<&gfx_value::set_member>("setMember");
      cls.func<&gfx_value::invoke>("invoke");
      cls.func<&gfx_value::invoke_result>("invokeResult");

      cls.func<&gfx_value::get_array_size>("getArraySize");
      cls.func<&gfx_value::set_array_size>("setArraySize");
      cls.func<&gfx_value::get_element>("getElement");
      cls.func<&gfx_value::set_element>("setElement");
      cls.func<&gfx_value::push_back>("pushBack");

      cls.func<&gfx_value::set_text>("setText");
      cls.func<&gfx_value::set_text_html>("setTextHTML");
    }

  private:
    RE::GFxValue value_;
  };
}
