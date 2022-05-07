//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
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


// Specialization types in this file:
//  int
//  unsigned int
//  bool
//  double
//  float
//  string
//  long long
//  unsigned char
//  signed char
//  char
//  unsigned short


using std::istringstream;


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< INT > /////////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<int>.
/**
 * Default value = 0.
 */
template<>
class gs_param<int>
: public gs_param_t<int>
{
  /// Typedef for the value.
  typedef int val_type;

  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;

  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;

  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("int");
  }

  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_INT;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const{
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< ss.str().c_str() << "'");
    if (ss.str().length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: set default value");
      return true;
    }
    val_type tmp;
    ss >> tmp;
    // if next char is a decimal point, ignore
    if (!ss.eof() && ss.peek() == '.') {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(int) ignored decimal point");
      return true;
    }
    // if error try hex
    if (!ss.eof() || ss.fail() || ss.bad()) {
      GS_PARAM_DUMP_WITHNAME("gs_param", "stream fail or not eof, try hex");
      ss.~istringstream();
      new ( (void *) &ss ) std::istringstream(str); // TODO: changed m_api->getParam(m_par_name) to str; correct??
      ss >> (std::hex) >> tmp;
    }
    // no conversion error
    if (ss.eof() && !ss.fail() && !ss.bad()) {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(int) stream eof and not fail"); 
    }  
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }    

};




// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< UNSIGNED INT > ////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<unsigned int>.
/**
 * Default value = 0.
 */
template<>
class gs_param<unsigned int>
: public gs_param_t<unsigned int>
{
  /// Typedef for the value.
  typedef unsigned int val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;

  explicit gs_param(const std::string &nam, const int &val) : gs_param_t<val_type>(nam        )  { gs_param_t<unsigned int>::init((unsigned int)val);  }
  explicit gs_param(const char *nam,        const int &val) : gs_param_t<val_type>(string(nam))  { gs_param_t<unsigned int>::init((unsigned int)val);  }

  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;


  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("unsigned int");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_UINT;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='" << ss.str().c_str() <<"'");
    if (ss.str().length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: set default value");
      return true;
    }
    val_type tmp;
    ss >> tmp;
    // if next char is a decimal point, ignore
    if (!ss.eof() && ss.peek() == '.') {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(unsigned int) ignored decimal point");
      return true;
    }
    // if error try hex
    if (!ss.eof() || ss.fail() || ss.bad()) {
      ss.~istringstream();
      new ( (void *) &ss ) std::istringstream(str); // TODO: changed m_api->getParam(m_par_name) to str; correct??
      ss >> (std::hex) >> tmp;
    }
    // no conversion error
    if (ss.eof() && !ss.fail() && !ss.bad()) {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(unsigned int) stream eof and not fail"); 
    }
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }

};




// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< BOOL > ////////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<bool>.
/**
 * Default value = false.
 */
template<>
class gs_param<bool>
: public gs_param_t<bool>
{
  /// Typedef for the value.
  typedef bool val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;

  // ///////////////////////
  //  operators
  
  // unsafe for bool: (according to MSVS)
  //GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  // unsafe for bool: (according to MSVS)
  //GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  //GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  inline my_type&                                                    
  operator ++ ()                                                     
  {                                                                  
    int tmp_val = getValue();                                   
    ++tmp_val;    						     
    this->setValue( (bool)tmp_val );                                       
    return *this;                                                    
  }                                                                  
                                                                     
  inline val_type                                                    
  operator ++ (int)                                                  
  {                                                                  
    val_type tmp_old_val = getValue();                               
    int tmp_set_val = tmp_old_val;                              
    tmp_set_val++;                                                   
    this->setValue((bool)tmp_set_val);                                    
    return tmp_old_val;                                              
  }  
  // not allowed for bool: 
  // GC_SPECIALISATIONS_DECREMENT_OPERATORS;

  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("bool");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_BOOL;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }

  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<<str.c_str()<<"'");
    if (str.length() == 0) {
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value (false)");
      target_val = false;
    }
    else if (strcmp( str.c_str(), "true" ) == 0) {
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set value true");
      target_val = true;
    }
    else if (strcmp( str.c_str(), "false" ) == 0) {
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set value false");
      target_val = false;
    }
    else {
      std::istringstream ss(str);
      //ss.setf(ios::boolalpha);
      val_type tmp;
      ss >> tmp;
      // no conversion error
      if (ss.eof() && !ss.fail() && !ss.bad()) {
        target_val = tmp;
        GS_PARAM_DUMP_WITHNAME("gs_param", "(bool) stream eof and not fail"); 
      }
      else {
        std::stringstream ess;
        ess << "Conversion error: '" << str << "'";
        SC_REPORT_WARNING("deserialize", ess.str().c_str());
        return false;
      }
    }
    return true;
  }


};




// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< DOUBLE > //////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<double>.
/**
 * Default value = 0.
 */
template<>
class gs_param<double>
: public gs_param_t<double>
{
  /// Typedef for the value.
  typedef double val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;

  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  // not allowed for double:
  // GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;

  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("double");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_DOUBLE;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    if (ss.str().length() == 0) {
      target_val = 0;
      return true;
    }
    val_type tmp;
    ss >> tmp;
    // no conversion error
    if (ss.eof() && !ss.fail() && !ss.bad()) {
      target_val = tmp;
    }
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }

};




// /////////////////////////////////////////////////////////////////////////////// //
// //////////////////  gs_param< FLOAT >  //////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<float>.
/**
 * Default value = 0.
 */
template<>
class gs_param<float>
: public gs_param_t<float>
{
  /// Typedef for the value.
  typedef float val_type;

  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;

  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  // not allowed for float:
  // GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;

  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("float");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_FLOAT;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    if (ss.str().length() == 0) {
      target_val = 0;
      return true;
    }
    val_type tmp;
    ss >> tmp;
    // no conversion error
    if (ss.eof() && !ss.fail() && !ss.bad()) {
      target_val = tmp;
    }
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }

};




// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< STRING > //////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //


/// Template specialization for gs_param<std::string>.
/**
 * 
 */
template<>
class gs_param<std::string>
: public gs_param_t<std::string>
{
protected:
  /// Typedef for this specialized class.
  typedef gs_param<std::string> my_type;
  /// Typedef for the value.
  typedef std::string val_type;

  using gs_param_t<val_type>::my_value;
  using gs_param_t<val_type>::m_api;
  using gs_param_t<val_type>::m_par_name;

  using gs_param_t<val_type>::convertStringToValue;

  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

private:
  // String constructor is allowed for string parameter

public:
  // Explicit constructors to avoid implicit construction of parameters.

  /// Empty constructor. Name will be set in base
  explicit gs_param() : gs_param_t<val_type>() { gs_param_t<val_type>::init();  }

  /// Constructor with (local/hierarchical) name.
  explicit gs_param(const std::string &nam) : gs_param_t<val_type>(nam             ) { gs_param_t<val_type>::init(); } 
  explicit gs_param(const char *nam       ) : gs_param_t<val_type>(std::string(nam)) { gs_param_t<val_type>::init(); } 

  /// Constructor with (local/hierarchical) name and string representation of initial value.
  explicit gs_param(const std::string &nam, const std::string &val, const bool force_top_level_name = false) : gs_param_t<val_type>(nam             , force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(val));         }
  explicit gs_param(const char *nam,        const char *val       , const bool force_top_level_name = false) : gs_param_t<val_type>(std::string(nam), force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit gs_param(const std::string &nam, const char *val       , const bool force_top_level_name = false) : gs_param_t<val_type>(nam             , force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit gs_param(const char *nam,        const std::string &val, const bool force_top_level_name = false) : gs_param_t<val_type>(std::string(nam), force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(val));         } 

  // Constructors with parent array
  explicit gs_param(                                                gs_param_array* parent_array) : gs_param_t<val_type>(                  false,  parent_array, true) { gs_param_t<val_type>::init(); }
  explicit gs_param(const std::string &nam                        , gs_param_array* parent_array) : gs_param_t<val_type>(nam             , false,  parent_array, true) { gs_param_t<val_type>::init(); } 
  explicit gs_param(const char *nam                               , gs_param_array* parent_array) : gs_param_t<val_type>(std::string(nam), false,  parent_array, true) { gs_param_t<val_type>::init(); } 
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array) : gs_param_t<val_type>(nam             , false,  parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         }
  explicit gs_param(const char *nam,        const char *val       , gs_param_array* parent_array) : gs_param_t<val_type>(std::string(nam), false,  parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(std::string(val))); } 
  explicit gs_param(const std::string &nam, const char *val       , gs_param_array* parent_array) : gs_param_t<val_type>(nam             , false,  parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(std::string(val))); } 
  explicit gs_param(const char *nam,        const std::string &val, gs_param_array* parent_array) : gs_param_t<val_type>(std::string(nam), false,  parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit gs_param(                                                gs_param_array& parent_array) : gs_param_t<val_type>(                  false, &parent_array, true) { gs_param_t<val_type>::init(); }
  explicit gs_param(const std::string &nam                        , gs_param_array& parent_array) : gs_param_t<val_type>(nam             , false, &parent_array, true) { gs_param_t<val_type>::init(); } 
  explicit gs_param(const char *nam                               , gs_param_array& parent_array) : gs_param_t<val_type>(std::string(nam), false, &parent_array, true) { gs_param_t<val_type>::init(); } 
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array) : gs_param_t<val_type>(nam             , false, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         }
  explicit gs_param(const char *nam,        const char *val       , gs_param_array& parent_array) : gs_param_t<val_type>(std::string(nam), false, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(std::string(val))); } 
  explicit gs_param(const std::string &nam, const char *val       , gs_param_array& parent_array) : gs_param_t<val_type>(nam             , false, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(std::string(val))); } 
  explicit gs_param(const char *nam,        const std::string &val, gs_param_array& parent_array) : gs_param_t<val_type>(std::string(nam), false, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } 

  // Constructors with register_at_db bool
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_t<val_type>(nam, force_top_level_name,  parent_array, register_at_db) { gs_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_t<val_type>(nam, force_top_level_name, &parent_array, register_at_db) { gs_param_t<val_type>::init(convertStringToValue(val));         } 

  const val_type convertStringToValue(const std::string& str) {    
    return str; 
  }                                                                 
  void serialize(const val_type &val) {                             
    my_value = val;                                      
  }
  
  /// Overloads gs_param_t<T>::deserialize in gs_param_t<T>
  bool deserialize(val_type &target_val, const std::string& str) {
    // Check the lock!
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    if (gs_param_base::m_locked) {                                                
      GS_PARAM_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;                                                               
    }                                                                             
#endif
    return static_deserialize(target_val, str);
  }
  
  /// To resolve the correct = operator
  using gs_param_t<val_type>::operator =;
  // TODO: other operators??

  using gs_param_base::name;
  using gs_param_t<val_type>::setString;
  using gs_param_t<val_type>::getString;
  using gs_param_t<val_type>::setValue;
  using gs_param_t<val_type>::getValue;

  /// Set the value of this parameter with char.
  /**
   * @param v  Char value which has to be set.
   * @return   Pointer to this.
   */
  my_type& operator = (const char *v) { 
    setValue(envvar_subst(v, m_par_name));
    return *this;
  }

  /// Desctructor
  ~gs_param() { gs_param_t<val_type>::destruct_gs_param(); }

  // ///////////////////////
  //  operators
  // see outside the class definition

  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("string");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_STRING;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return val;
  }  
  inline static std::string static_serialize(const val_type &val) {
    return val;//m_api->setParam(m_par_name, val);
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    target_val = str;
    return true;
  }

};

  
#define CNSC_STRING_OPERATOR_IMPLEMENTATIONS(otype)                     \
  inline bool operator otype (const gs_param<string>& lhs,              \
                              const gs_param<string>& rhs)              \
  {                                                                     \
    return const_cast<gs_param<string>& >(lhs).getValue() otype         \
      const_cast<gs_param<string>& >(rhs).getValue();                   \
  }                                                                     \
                                                                        \
  inline bool operator otype (const string& lhs, const gs_param<string>& rhs) \
  {                                                                     \
    return lhs otype const_cast<gs_param<string>& >(rhs).getValue();    \
  }                                                                     \
                                                                        \
  inline bool operator otype (const gs_param<string>& lhs, const string& rhs) \
  {                                                                     \
    return const_cast<gs_param<string>& >(lhs).getValue() otype rhs;    \
  }                                                                     \
                                                                        \
                                                                        \
                                                                        \
  inline bool operator otype (const char *lhs, const gs_param<string>& rhs) \
  {                                                                     \
    return string(lhs) otype const_cast<gs_param<string>& >(rhs).getValue(); \
  }                                                                     \
                                                                        \
  inline bool operator otype (const gs_param<string>& lhs, const char *rhs) \
  {                                                                     \
    return const_cast<gs_param<string>& >(lhs).getValue() otype string(rhs); \
  }                                                                     \
    
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(==)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(!=)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(<)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(<=)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(>)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(>=)
  
#undef CNSC_STRING_OPERATOR_IMPLEMENTATIONS

  
inline std::ostream& operator << (std::ostream& os, const gs_param<string>& str)
{
  os << const_cast<gs_param<string>& >(str).getValue();
  return os;
}

// TODO: check if works!
inline std::istream& operator>>(std::istream& is, gs_param<std::string>& str)
{
  string ret_str(const_cast<gs_param<std::string>& >(str).getValue());
  is >> ret_str;
  const_cast<gs_param<string>& >(str).setString(ret_str);
  return is;
}
  
inline std::string operator+ (const gs_param<std::string>& lhs, const gs_param<std::string>& rhs)
{
  return const_cast<gs_param<std::string>& >(lhs).getValue() + const_cast<gs_param<std::string>& >(rhs).getValue();
}
  
inline std::string operator+ (const gs_param<std::string>& lhs, const std::string& rhs)
{
  return const_cast<gs_param<std::string>& >(lhs).getValue() + rhs;
}

inline std::string operator+ (const std::string& lhs, const gs_param<std::string>& rhs)
{
  return lhs + const_cast<gs_param<std::string>& >(rhs).getValue();
}
 
inline std::string operator+ (const gs_param<std::string>& lhs, const char *rhs)
{
  return const_cast<gs_param<std::string>& >(lhs).getValue() + std::string(rhs);
}

inline std::string operator+ (const char *lhs, const gs_param<std::string>& rhs)
{
  return std::string(lhs) + const_cast<gs_param<std::string>& >(rhs).getValue();
}
 
 
// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< UNSIGNED LONG LONG > //////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<unsigned long long int>.
/**
 * Default value = 0.
 */
template<>
class gs_param<unsigned long long>
: public gs_param_t<unsigned long long>
{
  /// Typedef for the value.
  typedef unsigned long long val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;
  /// Constructor with unsigned int value
  //explicit gs_param(const std::string &nam, const unsigned long &val) : gs_param_t<val_type>(nam         )  { gs_param_t<val_type>::init((val_type)val); }    
  //explicit gs_param(const char *nam,        const unsigned long &val) : gs_param_t<val_type>(string(nam) )  { gs_param_t<val_type>::init((val_type)val); }    
  /// Constructor with int value
  //explicit gs_param(const std::string &nam, const long &val) : gs_param_t<val_type>(nam         )  { gs_param_t<val_type>::init((val_type)val); }    
  //explicit gs_param(const char *nam,        const long &val) : gs_param_t<val_type>(string(nam) )  { gs_param_t<val_type>::init((val_type)val); }    
  
  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("unsigned long long");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_ULONGLONG;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    if (ss.str().length() == 0) {
      target_val = 0;
      return true;
    }
    val_type tmp;
    ss >> tmp;
    // if next char is a decimal point, ignore
    if (!ss.eof() && ss.peek() == '.') {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(unsigned long long) ignored decimal point");
      return true;
    }
    // if error try hex
    if (!ss.eof() || ss.fail() || ss.bad()) {
      ss.clear(); ss.str(str);
      ss >> (std::hex) >> tmp;
    }
    // no conversion error
    if (ss.eof() && !ss.fail() && !ss.bad()) {
      target_val = tmp;
    } 
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }
  
};



// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< LONG LONG > //////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<long long int>.
/**
 * Default value = 0.
 */
template<>
class gs_param<long long>
: public gs_param_t<long long>
{
  /// Typedef for the value.
  typedef long long val_type;

  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;
  /// Constructor with int value
  explicit gs_param(const std::string &nam, const int &val) : gs_param_t<val_type>(nam         )  { gs_param_t<val_type>::init(val); }    
  explicit gs_param(const char *nam,        const int &val) : gs_param_t<val_type>(string(nam) )  { gs_param_t<val_type>::init(val); }    
  
  
  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("long long");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_LONGLONG;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
    
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    if (ss.str().length() == 0) {
      target_val = 0;
      return true;
    }
    val_type tmp;
    ss >> tmp;
    // if next char is a decimal point, ignore
    if (!ss.eof() && ss.peek() == '.') {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(long long) ignored decimal point");
      return true;
    }
    // if error try hex
    if (!ss.eof() || ss.fail() || ss.bad()) {
      ss.clear(); ss.str(str);
      ss >> (std::hex) >> tmp;
    }
    // if still error try deserialize to unsigned long long
    if (!ss.eof() || ss.fail() || ss.bad()) {
      ss.clear(); ss.str(str);
      unsigned long long ulltmp;
      ss >> ulltmp;
      if (!ss.eof() || ss.fail() || ss.bad()) {
        ss.clear(); ss.str(str);
        ss >> (std::hex) >> ulltmp;
      }
      if (ss.eof() && !ss.fail() && !ss.bad()) {
        tmp = ulltmp;
      }
    }
    
    // no conversion error
    if (ss.eof() && !ss.fail() && !ss.bad()) {
      target_val = tmp;
    } 
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }
  
};



// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< UNSIGNED CHAR > //////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<unsigned char>.
/**
 * Default value = 0.
 * @todo Regression tests to be done!
 */
template<>
class gs_param<unsigned char>
: public gs_param_t<unsigned char>
{
  /// Typedef for the value.
  typedef unsigned char val_type;

  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;

  // To resolve the correct = operator
  //using gs_param_t<val_type>::operator =;
  my_type& operator = (const val_type& v) { 
    //cout << "operator = (val_type) value="<<serialize(v) <<endl;
    setValue(v);
    return *this;
  }

  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;

  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("unsigned char");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_UCHAR;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << (int) val;
    return ss.str();
  }

  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    if (ss.str().length() == 0) {
      target_val = 0;
      return true;
    }

    int tmp=0;
    ss >> tmp;
    
    if (tmp >= 0 && tmp <= 255) {
      target_val = (unsigned char) tmp;
      return true;
    }
        
    // if error try hex
    if (!ss.eof() || ss.fail() || ss.bad()) {
      ss.clear(); ss.str(str);
      ss >> (std::hex) >> tmp;
    }
    // if still error, try characters 'x'
    if (!ss.eof() || ss.fail() || ss.bad()) {
      if (str.length() == 3 && str[0] == '\'' && str.at(2) == '\'') {
        target_val = str.at(1);
        return true;
      }
    }
    
    // no conversion error
    if ((ss.eof() && !ss.fail() && !ss.bad())  && (tmp >= 0 && tmp <= 255)) {
      target_val = tmp;
    }
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }

};

// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SIGNED CHAR > /////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<signed char>.
/**
 * Default value = 0.
 * @todo Regression tests to be done!
 */
template<>
class gs_param<signed char>
: public gs_param_t<signed char>
{
  /// Typedef for the value.
  typedef signed char val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;
  
  // To resolve the correct = operator
  //using gs_param_t<val_type>::operator =;
  my_type& operator = (const val_type& v) { 
    //cout << "operator = (val_type) value="<<serialize(v) <<endl;
    setValue(v);
    return *this;
  }
  
  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("signed char");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SIGNED_CHAR;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << (int) val;
    return ss.str();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    if (ss.str().length() == 0) {
      target_val = 0;
      return true;
    }
    
    int tmp=0;
    ss >> tmp;
    
    if (tmp <= 127 && tmp >= -128) {
      target_val = (signed char) tmp;
      return true;
    }
    
    // if error try hex
    if (!ss.eof() || ss.fail() || ss.bad()) {
      ss.clear(); ss.str(str);
      ss >> (std::hex) >> tmp;
    }
    // if still error, try characters 'x'
    if (!ss.eof() || ss.fail() || ss.bad()) {
      if (str.length() == 3 && str[0] == '\'' && str.at(2) == '\'') {
        target_val = str.at(1);
        return true;
      }
    }
    // no conversion error
    if ((ss.eof() && !ss.fail() && !ss.bad()) && (tmp <= 127 && tmp >= -128)) {
      target_val = tmp;
    }
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< CHAR > ////////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<char>.
/**
 * Default value = 0.
 * @todo Regression tests to be done!
 */
template<>
class gs_param<char>
: public gs_param_t<char>
{
  /// Typedef for the value.
  typedef char val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;
  
  // To resolve the correct = operator
  //using gs_param_t<val_type>::operator =;
  my_type& operator = (const val_type& v) { 
    //cout << "operator = (val_type) value="<<serialize(v) <<endl;
    setValue(v);
    return *this;
  }
  
  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("char");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_CHAR;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);;
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << (int) val;
    return ss.str();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    // This is to warn the use once if he is using a char in a numeric way, which is not specified by the C standard
    static bool warned_arithmetic_usage = false;
    
    std::istringstream ss(str);
    if (ss.str().length() == 0) {
      target_val = 0;
      return true;
    }
      
    int tmp=0;
    ss >> tmp;
    
    if (tmp <= 127 && tmp >= -128) {
      if (!warned_arithmetic_usage) {
        SC_REPORT_WARNING("deserialize", "A 'char' parameter gets numbers assigned. Note that the behavior is not specified by the C standard, use 'signed char' or 'unsigned char' instead!");
        warned_arithmetic_usage = true;
      }
      target_val = (char) tmp;
      return true;
    }
    
    // if error try hex
    if (!ss.eof() || ss.fail() || ss.bad()) {
      ss.clear(); ss.str(str);
      ss >> (std::hex) >> tmp;
      if (ss.eof() && !ss.fail() && !ss.bad() && !warned_arithmetic_usage) {
        SC_REPORT_WARNING("deserialize", "You are using a 'char' parameter with numbers. Note that the behavior is not specified by the C standard, use 'signed char' or 'unsigned char' instead!");
        warned_arithmetic_usage = true;
      }
    }
    // if still error, try characters 'x'
    if (!ss.eof() || ss.fail() || ss.bad()) {
      if (str.length() == 3 && str[0] == '\'' && str.at(2) == '\'') {
        target_val = str.at(1);
        return true;
      }
    }
    // no conversion error
    if ((ss.eof() && !ss.fail() && !ss.bad()) && (tmp <= 127 && tmp >= -128)) {
      target_val = tmp;
    }
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'"; 
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< UNSIGNED SHORT > //////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<unsigned short>.
/**
 * Default value = 0.
 */
template<>
class gs_param<unsigned short>
: public gs_param_t<unsigned short>
{
  /// Typedef for the value.
  typedef unsigned short val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;
  
  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("unsigned short");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_USHORT;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const{
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< ss.str().c_str()<<"'");
    if (ss.str().length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: set default value");
      return true;
    }
    val_type tmp;
    ss >> tmp;
    // if next char is a decimal point, ignore
    if (!ss.eof() && ss.peek() == '.') {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(unsigned short) ignored decimal point");
      return true;
    }
    // if error try hex
    if (!ss.eof() || ss.fail() || ss.bad()) {
      GS_PARAM_DUMP_WITHNAME("gs_param", "stream fail or not eof, try hex");
      ss.~istringstream();
      new ( (void *) &ss ) std::istringstream(str); // TODO: changed m_api->getParam(m_par_name) to str; correct??
      ss >> (std::hex) >> tmp;
    }
    // no conversion error
    if (ss.eof() && !ss.fail() && !ss.bad()) {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(unsigned short) stream eof and not fail"); 
    }  
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }    
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// //////////////////////////// gs_param< SHORT > //////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<short>.
/**
 * Default value = 0.
 */
template<>
class gs_param<short>
: public gs_param_t<short>
{
  /// Typedef for the value.
  typedef short val_type;
  
public:
  GS_PARAM_HEAD;
  
  // ///////////////////////
  //  operators
  
  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("short");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SHORT;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const{
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< ss.str().c_str()<<"'");
    if (ss.str().length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: set default value");
      return true;
    }
    val_type tmp;
    ss >> tmp;
    // if next char is a decimal point, ignore
    if (!ss.eof() && ss.peek() == '.') {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(short) ignored decimal point");
      return true;
    }
    // if error try hex
    if (!ss.eof() || ss.fail() || ss.bad()) {
      GS_PARAM_DUMP_WITHNAME("gs_param", "stream fail or not eof, try hex");
      ss.~istringstream();
      new ( (void *) &ss ) std::istringstream(str); // TODO: changed m_api->getParam(m_par_name) to str; correct??
      ss >> (std::hex) >> tmp;
    }
    // no conversion error
    if (ss.eof() && !ss.fail() && !ss.bad()) {
      target_val = tmp;
      GS_PARAM_DUMP_WITHNAME("gs_param", "(short) stream eof and not fail"); 
    }  
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }    
  
};

