//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008-2009 : GreenSocs Ltd
//      http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// This file should be included twice: one time with and one time without the 
// define GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
// The define is automatically undefed at the end!

// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< sc_attribute<T> >           ///////////////////// //
// ////  and  //////// gs_param< sc_attribute<std::string> > ///////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_attribute<T> >.
#ifndef GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
template<typename T>
class gs_param< sc_core::sc_attribute<T> >
#else
template <>
class gs_param< sc_core::sc_attribute<std::string> >
#endif
: public gs_param_base
{
protected:
#ifdef GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
  typedef std::string T;
#endif
  /// Typedef for the value.
  typedef sc_core::sc_attribute<T> val_type;
  /// Typedef for this specialized class.
  typedef gs_param<val_type> my_type;

  /// Value of this parameter
  val_type my_value;
  
  /// String whose reference can be returned as string value
  mutable std::string return_string;
  
  using gs_param_base::m_api;
  using gs_param_base::m_par_name;

private:
  /// Not allowed constructor!
  explicit gs_param(const val_type &val) : gs_param_base("", false, NULL, false), my_value("") { assert(false); }
  
public:
  // Explicit constructors to avoid implicit construction of parameters.
  
  /// Empty constructor. Name will be set in base
  explicit gs_param(const std::string &sc_attrib_name) : gs_param_base("", true, NULL, false), my_value(sc_attrib_name) { init();  }
  
  /// Constructor with (local/hierarchical) name.
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam) : gs_param_base(nam, true, NULL, false), my_value(sc_attrib_name) { init(); }
  explicit gs_param(const std::string &sc_attrib_name, const char *nam       ) : gs_param_base(string(nam), true, NULL, false), my_value(sc_attrib_name) { init(); }
  
  /// Constructor with (local/hierarchical) name and string representation of initial value.
  explicit gs_param(const std::string &sc_attrib_name, const char *nam,        const char *val       , const bool force_top_level_name = false) : gs_param_base(string(nam), true, NULL, force_top_level_name ), my_value(sc_attrib_name) { init(convertStringToValue(string(val))); } 
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const char *val       , const bool force_top_level_name = false) : gs_param_base(nam        , true, NULL, force_top_level_name ), my_value(sc_attrib_name) { init(convertStringToValue(string(val))); } 
#ifndef GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const std::string &val, const bool force_top_level_name = false) : gs_param_base(nam        , true, NULL, force_top_level_name ), my_value(sc_attrib_name) { init(convertStringToValue(val));         } 
  explicit gs_param(const std::string &sc_attrib_name, const char *nam,        const std::string &val, const bool force_top_level_name = false) : gs_param_base(string(nam), true, NULL, force_top_level_name ), my_value(sc_attrib_name) { init(convertStringToValue(val));         } 
#endif
  
  /// Constructor with (local/hierarchical) name and initial value.
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const T &val, const bool force_top_level_name = false) : gs_param_base(nam        , true, NULL, force_top_level_name ), my_value(sc_attrib_name)  { init(val); }    
  explicit gs_param(const std::string &sc_attrib_name, const char *nam,        const T &val, const bool force_top_level_name = false) : gs_param_base(string(nam), true, NULL, force_top_level_name ), my_value(sc_attrib_name)  { init(val); }    
  
  // Constructors with parent array
  explicit gs_param(const std::string &sc_attrib_name,                                                 gs_param_array* parent_array) : gs_param_base(""         , true, parent_array, false ), my_value(sc_attrib_name) { init(); }
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam                        , gs_param_array* parent_array) : gs_param_base(nam        , true, parent_array, false ), my_value(sc_attrib_name) { init(); } 
  explicit gs_param(const std::string &sc_attrib_name, const char *nam                               , gs_param_array* parent_array) : gs_param_base(string(nam), true, parent_array, false ), my_value(sc_attrib_name) { init(); } 
  explicit gs_param(const std::string &sc_attrib_name, const char *nam,        const char *val       , gs_param_array* parent_array) : gs_param_base(string(nam), true, parent_array, false ), my_value(sc_attrib_name) { init(convertStringToValue(string(val))); } 
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const char *val       , gs_param_array* parent_array) : gs_param_base(nam        , true, parent_array, false ), my_value(sc_attrib_name) { init(convertStringToValue(string(val))); } 
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const T &val   , gs_param_array* parent_array) : gs_param_base(nam        , true, parent_array, false ), my_value(sc_attrib_name) { init(val); }    
  explicit gs_param(const std::string &sc_attrib_name, const char *nam,        const T &val   , gs_param_array* parent_array) : gs_param_base(string(nam), true, parent_array, false ), my_value(sc_attrib_name) { init(val); }    
#ifndef GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const std::string &val, gs_param_array* parent_array) : gs_param_base(nam        , true, parent_array, false ), my_value(sc_attrib_name) { init(convertStringToValue(val));         } 
  explicit gs_param(const std::string &sc_attrib_name, const char *nam,        const std::string &val, gs_param_array* parent_array) : gs_param_base(string(nam), true, parent_array, false ), my_value(sc_attrib_name) { init(convertStringToValue(val));         } 
#endif
  
  // Constructors with parent array
  explicit gs_param(const std::string &sc_attrib_name,                                                 gs_param_array& parent_array) : gs_param_base(""         , true, &parent_array, false ), my_value(sc_attrib_name) { init(); }
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam                        , gs_param_array& parent_array) : gs_param_base(nam        , true, &parent_array, false ), my_value(sc_attrib_name) { init(); } 
  explicit gs_param(const std::string &sc_attrib_name, const char *nam                               , gs_param_array& parent_array) : gs_param_base(string(nam), true, &parent_array, false ), my_value(sc_attrib_name) { init(); } 
  explicit gs_param(const std::string &sc_attrib_name, const char *nam,        const char *val       , gs_param_array& parent_array) : gs_param_base(string(nam), true, &parent_array, false ), my_value(sc_attrib_name) { init(convertStringToValue(string(val))); } 
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const char *val       , gs_param_array& parent_array) : gs_param_base(nam        , true, &parent_array, false ), my_value(sc_attrib_name) { init(convertStringToValue(string(val))); } 
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const T &val   , gs_param_array& parent_array) : gs_param_base(nam        , true, &parent_array, false ), my_value(sc_attrib_name) { init(val); }    
  explicit gs_param(const std::string &sc_attrib_name, const char *nam,        const T &val   , gs_param_array& parent_array) : gs_param_base(string(nam), true, &parent_array, false ), my_value(sc_attrib_name) { init(val); }    
#ifndef GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const std::string &val, gs_param_array& parent_array) : gs_param_base(nam        , true, &parent_array, false ), my_value(sc_attrib_name) { init(convertStringToValue(val));         } 
  explicit gs_param(const std::string &sc_attrib_name, const char *nam,        const std::string &val, gs_param_array& parent_array) : gs_param_base(string(nam), true, &parent_array, false ), my_value(sc_attrib_name) { init(convertStringToValue(val));         } 
#endif
  
  // Constructors with register_at_db bool
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const std::string &val, gs_param_array* parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_base(nam, register_at_db,  parent_array, force_top_level_name ), my_value(sc_attrib_name) { init(convertStringToValue(val));         } 
  explicit gs_param(const std::string &sc_attrib_name, const std::string &nam, const std::string &val, gs_param_array& parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_base(nam, register_at_db, &parent_array, force_top_level_name ), my_value(sc_attrib_name) { init(convertStringToValue(val));         } 
  
  using gs_param_base::name;
  
  /// Init method to set the value and add the parameter to the plugin db.
  virtual void init(const T &default_val) {
    // Init value member
    value.m_owner = this;
    GS_PARAM_DUMP("Init gs_param "<< m_par_name.c_str());
    // set my_value
    my_value.value = default_val;
    // add to plugin database
    if (m_register_at_db) {
      m_api->addPar(this);
    }
  }
  /// Init method without value @see gs::cnf::gs_param_t::init(T &default_val)
  virtual void init() {
    // Init value member
    value.m_owner = this;
    GS_PARAM_DUMP("Init gs_param "<< m_par_name.c_str());
    my_value.value = convertStringToValue(string("")); // TODO: use gs_param<T>::convertStringToValue
    // add to plugin database
    if (m_register_at_db) {
      m_api->addPar(this);
    }
  }
  
  /// Destructor
  virtual ~gs_param() {
    gs_param_base::destruct_gs_param();
  }
  
  

  // ///////////////////////
  //  additional functions
  
  /// Wrapper class for value member
  template<typename WT>
  struct wrapper {
    gs_param< sc_core::sc_attribute<WT> >* m_owner;
    wrapper(gs_param< sc_core::sc_attribute<WT> >* owner) : m_owner(owner) {  }
    wrapper() : m_owner(NULL) {  }
#ifndef GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
    wrapper<WT>& operator = ( const WT& _value) {
      assert(m_owner != NULL);
      m_owner->setValue(_value);
      return *this;
    }
    operator const WT () {
      assert(m_owner != NULL);
      return m_owner->getValue().value;
    }
#else
    wrapper<WT>& operator = ( const std::string& _value) {
      assert(m_owner != NULL);
      m_owner->setValue(envvar_subst(_value, m_owner->m_par_name));
      return *this;
    }
    operator std::string () {
      assert(m_owner != NULL);
      return m_owner->getValue().value;
    }
#endif
  };
  
  wrapper<T> value;
  
  // ///////////////////////
  //  overloaded functions

  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return std::string("sc_attribute<T>");
  }

  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_ATTRIBUTE;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const T &val) const{
    return gs_param<T>::static_serialize(val);
  }  
  
  /// Overloads gs_param_t<T>::deserialize in gs_param_t<T>
  bool deserialize(T &target_val, const std::string& str) {
    return static_deserialize(target_val, str);
  }
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(T &target_val, const std::string& str) {
    return gs_param<T>::static_deserialize(target_val, str);
  }   
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with value   ///////////////////////////////// //
  
  
  /// Set the value of this parameter to the value of another gs_param.
  my_type& operator = (const my_type& v) { 
    setValue(const_cast<my_type*>(&v)->getValue()); //m_api->setParam(m_par_name, const_cast<my_type&>(v).get());
    return *this;
  }
  
  /// Set the value of this parameter.
  my_type& operator = (const T& v) { 
    setValue(v);
    return *this;
  }

  /// Set the value of this parameter.
  my_type& operator = (const val_type& v) { 
    setValue(v);
    return *this;
  }
  
  /// Get the value of this parameter.
  operator const val_type& () { // const { 
    return getValue(); 
  }
  
  /// Set the value of this parameter.
  bool setValue(const val_type &val) {
    if (make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_DUMP("reject_write callback rejected value change!");
      return false;
    }
    make_pre_write_callbacks();
    my_value.value = val.value;
    make_post_write_callbacks();
    return true;
  }
  
  /// Set the value of this parameter.
  bool setValue(const T &val) {
    if (make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_DUMP("reject_write callback rejected value change!");
      return false;
    }
    make_pre_write_callbacks();
    my_value.value = val;
    make_post_write_callbacks();
    return true;
  }
  
  /// Returns the value of this parameter.
  const val_type& getValue() const {
    make_pre_read_callbacks();
    //static my_type tmp;
    //tmp = my_value
    make_post_read_callbacks(); // TODO Note: here the actual read had not yet been done!
    //return tmp;
    return my_value;
  }
  
  /// @see gs::cnf::gs_param_base::get_value_pointer
  const void* get_value_pointer() const {
    make_pre_read_callbacks();
    make_post_read_callbacks(); // TODO Note: here the actual read had not yet been done!
    return &my_value;
  }
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with string representation   ///////////////// //
  
  
  /// Set the value of this parameter with a string.
  bool setString(const std::string &str) {
    if (make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_DUMP("reject_write callback rejected value change!");
      return false;
    }
    make_pre_write_callbacks();
    bool success = deserialize(my_value.value, envvar_subst(str, m_par_name));
    make_post_write_callbacks();
    return success;
  }
  
  /// Get the value of this parameter as a string.
  const std::string& getString() const {
    make_pre_read_callbacks();
    return_string = serialize(my_value.value);
    make_post_read_callbacks(); // TODO Note: here the actual read had not yet been done!
    return return_string;
  }
  
  
protected:
  /// Get the value the string. Needed for construction of gs_param.
  const T& convertStringToValue(const std::string& str) {    
    deserialize(my_value.value, envvar_subst(str, m_par_name));
    return my_value.value;                              
  }         

};

#ifdef GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
inline std::ostream& operator << (std::ostream& os, gs_param<sc_core::sc_attribute<std::string> >::wrapper<std::string>& wr)
{
  os << (std::string)wr;
  return os;
}
#endif


#ifdef GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
#  undef GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
#endif
