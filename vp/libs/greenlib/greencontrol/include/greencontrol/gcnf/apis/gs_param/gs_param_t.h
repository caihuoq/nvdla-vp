//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxgen comments

#ifndef __GS_PARAM_T_h__
#define __GS_PARAM_T_h__

// included by gs_params.h
// #include "gs_param_base" is done there

#include "greencontrol/gcnf/apis/utils/envvar_subst.h"

namespace gs {
namespace cnf {


/// Template specialized base class for configuration parameters.
template <typename T>
class gs_param_t 
: public gs_param_base
{
  /// Typedef for this specialized class.
  typedef gs_param_t<T> my_type;

  /// Typedef for the value.
  typedef T val_type;

protected:
  /// Value of this parameter
  val_type my_value;
  
  /// Default value of this parameter
  val_type my_default_value;

  /// String whose reference can be returned as string value
  mutable std::string return_string;

  using gs_param_base::m_locked;
  using gs_param_base::m_lock_pwd;
  
public:
  using gs_param_base::set;

  // templated operators
  my_type& operator +=  (val_type);
  my_type& operator -=  (val_type);
  my_type& operator /=  (val_type);
  my_type& operator *=  (val_type);
  val_type operator +  (val_type);
  val_type operator -  (val_type);
  val_type operator /  (val_type);
  val_type operator *  (val_type);
  my_type& operator %=  (val_type);
  my_type& operator ^=  (val_type);
  my_type& operator &=  (val_type);
  my_type& operator |=  (val_type);
  my_type& operator <<= (val_type);
  my_type& operator >>= (val_type);

  // templated decrement and increment
  my_type& operator -- ();    // prefix
  val_type operator -- (int); // postfix
  my_type& operator ++ ();    // prefix
  val_type operator ++ (int); // postfix  


  // //////////////////////////////////////////////////////////////////// //
  // //////////////   constructors and destructor   ///////////////////// //


  /// Empty constructor. Name will be set in base. Avoid using it! You MUST call init() after instantiation!
  /**
   */
  explicit gs_param_t()
  : gs_param_base("", true, NULL, false)
  {  }
  
  /// Constructor with the special parameters. Name will be set in base. Avoid using it! You MUST call init() after instantiation!
  /**
   * @param register_at_db  see gs::cnf::gs_param_base, default = true.
   * @param parent_array    see gs::cnf::gs_param_base, default=NULL.
   * @param force_top_level_name If the given name n should be a top-level name (then no prefeix is attached to the name),
   *                             default false,
   *                             be carefull in using this.
   */
  explicit gs_param_t(const bool force_top_level_name, gs_param_array* parent_array, const bool register_at_db)
    : gs_param_base("", register_at_db, parent_array, force_top_level_name)
  {  }

  /// Constructor with (local or hierarchical) name. You MUST call init() after instantiation!
  /**
   * This constructor may be called with local or hierarchical name.
   *
   * Explicit constructor to avoid implicit construction of parameters.
   *
   * gs_param MUST call init() or init(value) after construction to 
   * add the parameter to the plugin database!!!
   *
   * @param nam  The local (or full hierarchical) parameter name (local: not including points) 
   *             (local: unique inside a module, hierarchical: unique in the system).
   */
  explicit gs_param_t(const std::string &nam)
    : gs_param_base(nam, true, NULL, false)
  {  }

  /// Constructor with (local or hierarchical) name and special parameters. You MUST call init() after instantiation!
  /**
   * This constructor may be called with local or hierarchical name.
   *
   * Explicit constructor to avoid implicit construction of parameters.
   *
   * gs_param MUST call init() or init(value) after construction to 
   * add the parameter to the plugin database!!!
   *
   * @param nam  The local (or full hierarchical) parameter name (local: not including points) 
   *             (local: unique inside a module, hierarchical: unique in the system).
   * @param register_at_db  see gs::cnf::gs_param_base, default = true.
   * @param parent_array    see gs::cnf::gs_param_base, default=NULL.
   * @param force_top_level_name If the given name n should be a top-level name (then no prefeix is attached to the name),
   *                             default false,
   *                             be carefull in using this.
   */
  explicit gs_param_t(const std::string &nam, const bool force_top_level_name,
                      gs_param_array* parent_array, const bool register_at_db)
  : gs_param_base(nam, register_at_db, parent_array, force_top_level_name)
  {  }
  
  /// Init method to set the value and add the parameter to the plugin db.
  /** 
   * Has to be called by the gs_param class after construction!
   * Does set the value and add the parameter to the plugin db.
   */
  void init(const val_type &default_val) {
    GS_PARAM_DUMP("Init gs_param_t "<< m_par_name.c_str());
    // set my_value
    my_value = default_val;
    // store default value
    my_default_value = default_val;
    // set m_default_value_was_set
    m_default_value_was_set = true;
    // add to plugin database
    if (m_register_at_db) {
      assert(m_api != NULL);
      m_api->addPar(this);
    }
  }
  /// Init method without value @see gs::cnf::gs_param_t::init(val_type &default_val)
  void init() {
    GS_PARAM_DUMP("Init gs_param_t "<< m_par_name.c_str());
    // set m_default_value_was_set
    m_default_value_was_set = false;
    my_value = convertStringToValue(string(""));
    // add to plugin database
    if (m_register_at_db) {
      assert(m_api != NULL);
      m_api->addPar(this);
    }
  }

  /// Destructor
  virtual ~gs_param_t() {
  }


  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with value   ///////////////////////////////// //


  /// Set the value of this parameter to the value of another gs_param.
  /**
   * To resolve the correct = operator in the (not specialized) class
   * which inherits from this class has to use 
   * \code using gs_param_t<val_type>::operator =; \endcode
   *
   * @param v  Parameter where the value should be read from.
   * @return   Pointer to this.
   */
  my_type& operator = (const my_type& v) { 
    setValue(v.getValue()); //m_api->setParam(m_par_name, const_cast<my_type&>(v).get());
    return *this;
  }

  /// Set the value of this parameter.
  /**
   * To resolve the correct = operator in the (not specialized) class
   * which inherits from this class has to use 
   * \code using gs_param_t<val_type>::operator =; \endcode
   *
   * @param v  Value which has to be set.
   * @return   Pointer to this.
   */
  my_type& operator = (const val_type& v) { 
    setValue(v);
    return *this;
  }

  /// Get the value of this parameter.
  /**
   * @return Value of the parameter.
   */
  operator const val_type& () const { 
    return getValue(); 
  }

  /// Set the value of this parameter.
  /**
   * Calls conversion value type --> string.
   *
   * @param val  The new value for this parameter.
   * @return If the parameter was set successfully (false if it has been rejected)
   */
  bool setValue(const val_type &val) {
    if (make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_DUMP("reject_write callback rejected value change!");
      return false;
    }
    make_pre_write_callbacks();
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    if (gs_param_base::m_locked) {                                                
      GS_PARAM_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;                                                               
    }                           
#endif
    my_value = val;
    make_post_write_callbacks();
    return true;
  }
  
  // Set function overriding the lock
  bool setValue(const val_type& val, void* lock_pwd) {                                 
    if (gs_param_base::m_locked && gs_param_base::m_lock_pwd != lock_pwd) {
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked with different password than given one!");
      return false;                                                                     
    }
    if (make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_DUMP("reject_write callback rejected value change!");
      return false;
    }
    make_pre_write_callbacks();
    my_value = val;
    make_post_write_callbacks();
    return true;
  }
  
  /// Returns the value of this parameter.
  /**
   * @return Value
   */
  const val_type& getValue() const {
    make_pre_read_callbacks();
    make_post_read_callbacks(); // TODO Note: here the actual read had not yet been done!
    return my_value;
  }
  
  /// @see gs::cnf::gs_param_base::get_value_pointer
  const void* get_value_pointer() const{
    make_pre_read_callbacks();
    make_post_read_callbacks(); // TODO Note: here the actual read had not yet been done!
    return &my_value;
  }

  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with string representation   ///////////////// //


  /// Set the value of this parameter with a string.
  /**
   * @param str  The new value for this parameter, represented as a string.
   * @return If setting was successful
   */ 
  bool setString(const std::string &str) {
    if (make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_DUMP("reject_write callback rejected value change!");
      return false;
    }
    make_pre_write_callbacks();
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    if (gs_param_base::m_locked) {                                                
      GS_PARAM_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;                                                               
    }                           
#endif
    bool success = deserialize(my_value, envvar_subst(str, m_par_name));
    if (success) make_post_write_callbacks();
    return success;
  }
  
  /// Get the value of this parameter as a string.
  /**
   * @return String representation of the current value of this parameter.
   */
  const std::string& getString() const {
    make_pre_read_callbacks();
    return_string = serialize(my_value);
    make_post_read_callbacks();
    return return_string;
  }
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////   virtual conversion methods string <-> value   //////////// //


  /// Conversion method value type --> string. To be implemented by the specialization.
  /**
   * May not make use of m_par_name because it is called inside constructor!
   *
   * @param val  Value that should be converted to string.
   * @return String representation of the value.
   */
  virtual std::string serialize(const val_type &val) const = 0;
  //virtual std::string convertValueToString(const val_type &val) const = 0; // OLD

  /// Deserialize for this parameter. To be implemented by the specialization.
  /**
   * Conversion string --> value type.
   *
   * User implemented for each template specialization of gs_param:
   * Do not write to target_val if deserialization fails!
   *
   * Set target_val to the default value if str is empty (=="").
   *
   * Do not use ANY member variables. Act as if the method is static.
   *
   * @param  target_val  Reference to the value that should be set.
   * @param  str         String that should be converted to a value.
   * @return If the convertion was successfull
   */
  virtual bool deserialize(val_type &target_val, const std::string& str) = 0;

  
  // //////////////////////////////////////////////////////////////////// //
  // ///////////////   Get default Value   ////////////////////////////// //
  
  
  /// Get the parameter's original default value
  /**
   * The returned value is uninitialized if the default had not been set, so please test gs_param_base::has_default_value before!
   *
   * @return This value is the default value being set in the constructor
   */
  virtual const val_type& get_default_value() {
    return my_default_value;
  }
  
protected:
  /// Get the value the string. Needed for construction of gs_param.
  /**
   * Conversion string --> value type.
   *
   * @param  str String that should be converted to a value
   * @return If the convert was successfull
   */
  const val_type& convertStringToValue(const std::string& str) {    
    deserialize(my_value, envvar_subst(str, m_par_name));
    return my_value;                              
  }                                                                 

};


} // end namespace cnf
} // end namespace gs

#endif
