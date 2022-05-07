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
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>,
//   Jan Tuttas <jan@tuttas.eu>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//   Marius Montón <marius.monton@uab.cat>,
//     Hardware-Software Prototypes and Solutions Lab (CEPHIS). 
//     Microelectronics and Electronics SystemS Dpt. UAB.
//     http://cephis.uab.cat
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


// Specialization types in this file:
//  sc_int_base
//  sc_int<w>
//  sc_uint_base
//  sc_uint<w>
//  sc_signed
//  sc_bigint<w>
//  sc_unsigned
//  sc_biguint<w>
//  sc_bit
//  sc_logic
//  sc_time
//  sc_event


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SC_INT_BASE > ////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_int_base >.
/**
 * Default value = 0.
 */
template<>
class gs_param< sc_dt::sc_int_base >
: public gs_param_t< sc_dt::sc_int_base >
{
  /// Typedef for the value.
  typedef sc_dt::sc_int_base val_type;
  
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
    return string("sc_int_base");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_INT_BASE;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    return val.to_string();
  }
   
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
    if (str.length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value");
      return true;
    }
    target_val = str.c_str();
    return true;
  }
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SC_INT<w> > ///////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_int<W> >.
/**
 * Default value = 0.
 */
template<int W>
class gs_param< sc_dt::sc_int<W> >
: public gs_param_t< sc_dt::sc_int<W> >
{
  /// Typedef for the value.
  typedef sc_dt::sc_int<W> val_type;

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
    return string("sc_int");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_INT;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    return val.to_string();
  }

  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
    if (str.length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value");
      return true;
    }
    target_val = str.c_str();
    return true;
  }

};

// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SC_UINT_BASE > ////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_uint_base >.
/**
 * Default value = 0.
 */
template<>
class gs_param< sc_dt::sc_uint_base >
: public gs_param_t< sc_dt::sc_uint_base >
{
  /// Typedef for the value.
  typedef sc_dt::sc_uint_base val_type;
  
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
    return string("sc_uint_base");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_UINT_BASE;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    return val.to_string();
  }
    
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
    if (str.length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value");
      return true;
    }
    target_val = str.c_str();
    return true;
  }
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SC_UINT<w> > //////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_uint<W> >.
/**
 * Default value = 0.
 */
template<int W>
class gs_param< sc_dt::sc_uint<W> >
: public gs_param_t< sc_dt::sc_uint<W> >
{
  /// Typedef for the value.
  typedef sc_dt::sc_uint<W> val_type;

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
    return string("sc_uint");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_UINT;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    return val.to_string();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
    if (str.length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value");
      return true;
    }
    target_val = str.c_str();
    return true;
  }
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SC_SIGNED > ///////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_signed >.
/**
 * Default value = 0.
 */
template<>
class gs_param< sc_dt::sc_signed >
: public gs_param_t< sc_dt::sc_signed >
{
  /// Typedef for the value.
  typedef sc_dt::sc_signed val_type;
  
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
    return string("sc_signed");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_SIGNED;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    return val.to_string();
  }
    
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
    if (str.length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value");
      return true;
    }
    target_val = str.c_str();
    return true;
  }
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SC_BIGINT<w> > ////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_bigint<W> >.
/**
 * Default value = 0.
 */
template<int W>
class gs_param< sc_dt::sc_bigint<W> >
: public gs_param_t< sc_dt::sc_bigint<W> >
{
  /// Typedef for the value.
  typedef sc_dt::sc_bigint<W> val_type;
  
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
    return string("sc_bigint");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_BIGINT;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    return val.to_string();
  }
    
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
    if (str.length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value");
      return true;
    }
    target_val = str.c_str();
    return true;
  }
  
};

// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SC_UNSIGNED > /////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_unsigned >.
/**
 * Default value = 0.
 */
template<>
class gs_param< sc_dt::sc_unsigned >
: public gs_param_t< sc_dt::sc_unsigned >
{
  /// Typedef for the value.
  typedef sc_dt::sc_unsigned val_type;
  
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
    return string("sc_unsigned");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_UNSIGNED;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    return val.to_string();
  }
    
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
    if (str.length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value");
      return true;
    }
    target_val = str.c_str();
    return true;
  }
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SC_BIGUINT<w> > ///////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_biguint<W> >.
/**
 * Default value = 0.
 */
template<int W>
class gs_param< sc_dt::sc_biguint<W> >
: public gs_param_t< sc_dt::sc_biguint<W> >
{
  /// Typedef for the value.
  typedef sc_dt::sc_biguint<W> val_type;
  
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
    return string("sc_biguint");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_BIGUINT;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    return val.to_string();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
    if (str.length() == 0) {
      target_val = 0;
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value");
      return true;
    }
    target_val = str.c_str();
    return true;
  }
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// ///////////////////  gs_param< sc_bit >  ////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_bit >.
/**
 * Default value = SC_LOGIC_0.
 */
template<>
class gs_param<sc_dt::sc_bit>
: public gs_param_t<sc_dt::sc_bit>
{
  /// Typedef for the value.
  typedef sc_dt::sc_bit val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;
  
  explicit gs_param(const std::string &nam, const bool &val) : gs_param_t<val_type>(nam         )  { gs_param_t<val_type>::init((sc_dt::sc_bit)val); }
  explicit gs_param(const char *nam,        const bool &val) : gs_param_t<val_type>(string(nam) )  { gs_param_t<val_type>::init((sc_dt::sc_bit)val); }
  
  // ///////////////////////
  //  operators
  
  /// Operator = with sc_logic
  my_type& operator = (const sc_dt::sc_logic &v) { 
    setValue(v);
    return *this;
  }
  
  /// Operator == with sc_logic
  bool operator == (const sc_dt::sc_logic val2) {
    return getValue() == val2;
  }
  
  /// Set the value of this parameter specialized for sc_logic.
  /**
   * Calls conversion value type --> string.
   *
   * @param val  The new value for this parameter.
   */
  bool setValue(const sc_dt::sc_logic &val) {
    val_type tmp;
    tmp = val;
    return setValue(tmp);
  }
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("sc_bit");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_BIT;
  }
  
  /// serialize(sc_logic)
  std::string serialize(const sc_dt::sc_logic &v) {
    DEPRECATED_WARNING("gs_param<sc_bit>", "DEPRECATED: sc_bit is deprecated use bool instead.");
    if (v == sc_dt::SC_LOGIC_0)       return std::string("0");
    else if (v == sc_dt::SC_LOGIC_1)  return std::string("1");
    else SC_REPORT_WARNING(name(), "operator = error: no SC_LOGIC_0 or SC_LOGIC_1");
    return std::string("");
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    DEPRECATED_WARNING("gs_param<sc_bit>", "DEPRECATED: sc_bit is deprecated use bool instead.");
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
    
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    DEPRECATED_WARNING("gs_param<sc_bit>", "DEPRECATED: sc_bit is deprecated use bool instead.");
    if (str.length() == 0) {
      target_val = sc_dt::SC_LOGIC_0;
    }
    else if (strcmp( str.c_str(), "SC_LOGIC_1") == 0 || strcmp( str.c_str(), "1") == 0) {
      target_val = sc_dt::SC_LOGIC_1;
    }
    else if (strcmp( str.c_str(), "SC_LOGIC_0") == 0 || strcmp( str.c_str(), "0") == 0) {
      target_val = sc_dt::SC_LOGIC_0;
    }
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }
  
  // TODO << operator!
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// ///////////////////  gs_param< sc_logic >  //////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_logic >.
/**
 * Default value = SC_LOGIC_0.
 */
template<>
class gs_param< sc_dt::sc_logic >
: public gs_param_t< sc_dt::sc_logic >
{
  /// Typedef for the value.
  typedef sc_dt::sc_logic val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;
  
  explicit gs_param(const std::string &nam, const sc_dt::sc_logic_value_t val) : gs_param_t<val_type>(nam         )  { gs_param_t<val_type>::init((sc_dt::sc_logic)val); }
  explicit gs_param(const char *nam,        const sc_dt::sc_logic_value_t val) : gs_param_t<val_type>(string(nam) )  { gs_param_t<val_type>::init((sc_dt::sc_logic)val); }
  explicit gs_param(const std::string &nam, const bool &val) : gs_param_t<val_type>(nam              )  { gs_param_t<val_type>::init((sc_dt::sc_logic)val); }
  explicit gs_param(const char *nam,        const bool &val) : gs_param_t<val_type>(std::string(nam) )  { gs_param_t<val_type>::init((sc_dt::sc_logic)val); }
  explicit gs_param(const std::string &nam, const char &val) : gs_param_t<val_type>(nam              )  { gs_param_t<val_type>::init((sc_dt::sc_logic)val); }
  explicit gs_param(const char *nam,        const char &val) : gs_param_t<val_type>(std::string(nam) )  { gs_param_t<val_type>::init((sc_dt::sc_logic)val); }
  explicit gs_param(const std::string &nam, const int  &val) : gs_param_t<val_type>(nam              )  { gs_param_t<val_type>::init((sc_dt::sc_logic)val); }
  explicit gs_param(const char *nam,        const int  &val) : gs_param_t<val_type>(std::string(nam) )  { gs_param_t<val_type>::init((sc_dt::sc_logic)val); }
  
  // ///////////////////////
  //  operators
  
  /// Operator = with sc_logic_value_t
  my_type& operator = (const sc_dt::sc_logic_value_t v) { 
    setValue((sc_dt::sc_logic)v);
    return *this;
  }
  /// Operator == with sc_logic_value_t
  bool operator == (sc_dt::sc_logic_value_t val2) {
    return getValue() == (sc_dt::sc_logic)val2;
  }
  
  /// Operator = with bool
  my_type& operator = (const bool &v) { 
    setValue((sc_dt::sc_logic)v);
    return *this;
  }
  /// Operator == with bool
  bool operator == (const bool val2) {
    return getValue() == (sc_dt::sc_logic)val2;
  }
  
  /// Operator = with char
  my_type& operator = (const char &v) { 
    setValue((sc_dt::sc_logic)v);
    return *this;
  }
  /// Operator == with char
  bool operator == (const char val2) {
    return getValue() == (sc_dt::sc_logic)val2;
  }
  
  /// Operator = with int
  my_type& operator = (const int &v) { 
    setValue((sc_dt::sc_logic)v);
    return *this;
  }
  /// Operator == with int
  bool operator == (const int val2) {
    return getValue() == (sc_dt::sc_logic)val2;
  }
  
  /// Set the value of this parameter specialized for sc_logic.
  /**
   * Calls conversion value type --> string.
   *
   * @param val  The new value for this parameter.
   */
  /* TODO: isn't this in the base class? Otherwise repair make callback functions
   void setValue(const sc_dt::sc_logic &val) {
    my_value = val;
    makeCallbacks();
  }*/
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("sc_logic");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_LOGIC;
  }
  
  /// serialize(sc_logic)
  std::string serialize(const sc_dt::sc_logic &v) {
    if (v == sc_dt::SC_LOGIC_0)       return std::string("0");
    else if (v == sc_dt::SC_LOGIC_1)  return std::string("1");
    else if (v == sc_dt::SC_LOGIC_Z)  return std::string("Z");
    else if (v == sc_dt::SC_LOGIC_X)  return std::string("X");
    else SC_REPORT_WARNING(name(), "operator = error: no SC_LOGIC_0 or SC_LOGIC_1 or SC_LOGIC_Z or SC_LOGIC_X");
    return std::string("");
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }
    
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    if (str.length() == 0) {
      target_val = sc_dt::SC_LOGIC_0;
    }
    else if (strcmp( str.c_str(), "SC_LOGIC_1") == 0 || strcmp( str.c_str(), "1") == 0) {
      target_val = sc_dt::SC_LOGIC_1;
    }
    else if (strcmp( str.c_str(), "SC_LOGIC_0") == 0 || strcmp( str.c_str(), "0") == 0) {
      target_val = sc_dt::SC_LOGIC_0;
    }
    else if (strcmp( str.c_str(), "SC_LOGIC_Z") == 0 
             || strcmp( str.c_str(), "Z") == 0  || strcmp( str.c_str(), "z") == 0) {
      target_val = sc_dt::SC_LOGIC_Z;
    }
    else if (strcmp( str.c_str(), "SC_LOGIC_X") == 0
             || strcmp( str.c_str(), "X") == 0  || strcmp( str.c_str(), "x") == 0) {
      target_val = sc_dt::SC_LOGIC_X;
    }
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }
  
  // TODO << operator!
  
};


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< SC_TIME > /////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_time >.
/**
 * Default value = 0.
 */
template<>
class gs_param< sc_core::sc_time >
: public gs_param_t< sc_core::sc_time >
{
  /// Typedef for the value.
  typedef sc_core::sc_time val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  GS_PARAM_HEAD;
  
  // ///////////////////////
  //  operators
  
  //GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  //GC_SPECIALISATIONS_BINARY_OPERATORS;
  
  //GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  //GC_SPECIALISATIONS_DECREMENT_OPERATORS;
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("sc_time");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_TIME;
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    std::ostringstream ret (std::ostringstream::out);
    ret << val;
    return ret.str();
  }
    
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  // supports strings like "uint64 unit"
  // "56,23 us" or "0.124 ms"
  // also keeps support to "double" (menaning SEC)
  // "0.234"
    
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
    if (str.length() == 0) {
      target_val = sc_time(0, SC_SEC);
      GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: set default value");
      return true;
    }
    
    std::string::size_type index;
    double tmp_val;
    std::string tmp_str;
    
    index = str.find(" ");
    if (index != str.npos) {
      // case of "number <space> units"
      // first get the double
      tmp_str = str.substr(0, index);
      std::istringstream(tmp_str) >> tmp_val;
      // now tmp_val stores the value

      // time to manage units (ugly style)
      tmp_str = str.substr(index+1, str.npos);

      if (tmp_str.compare("s")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_SEC); return true;}
      if (tmp_str.compare("ms")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_MS); return true;}
      if (tmp_str.compare("us")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_US); return true;}
      if (tmp_str.compare("ns")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_NS); return true;}
      if (tmp_str.compare("ps")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_PS); return true;}
      if (tmp_str.compare("fs")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_FS); return true;}
      
      if (tmp_str.compare("SC_SEC")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_SEC); return true;}
      if (tmp_str.compare("SC_MS")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_MS); return true;}
      if (tmp_str.compare("SC_US")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_US); return true;}
      if (tmp_str.compare("SC_NS")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_NS); return true;}
      if (tmp_str.compare("SC_PS")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_PS); return true;}
      if (tmp_str.compare("SC_FS")==0) {target_val = sc_core::sc_time(tmp_val, sc_core::SC_FS); return true;}
      
      // If we are still here, we had a wrong input string
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
      
    } else {
      // handle the "value" (in SEC) string
      gs_param<double>::static_deserialize(tmp_val, str);
      target_val = sc_core::sc_time(tmp_val, sc_core::SC_SEC);
      return true;
    }
    return false;
  }
  
};




// /////////////////////////////////////////////////////////////////////////////// //
// ///////////////////  gs_param< sc_event >  //////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param< sc_event >.
/**
 * This specialization has special callback behavior
 */
template<>
class gs_param< sc_core::sc_event >
: public gs_param_t< sc_core::sc_event >
{
  /// Typedef for the value.
  typedef sc_core::sc_event val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

public:
  //GS_PARAM_HEAD;
  //do not use macro: need special init()-function, because sc_event can not store any value
protected:                                                             
  typedef gs_param<val_type> my_type;                                  
  using gs_param_t<val_type>::my_value;                                
  using gs_param_t<val_type>::m_api;                                   
  using gs_param_t<val_type>::m_par_name;                              
  using gs_param_t<val_type>::convertStringToValue; 
  val_type intern_event;
                      
private:                                                               
  explicit gs_param(const val_type &val) { sc_assert(false); }         
                                                                       
public:                                                                
  explicit gs_param() : gs_param_t<val_type>(false, NULL, true) { init();  }

  explicit gs_param(const std::string &nam) : gs_param_t<val_type>(nam        , false, NULL, true) { init(); } 
  explicit gs_param(const char *nam       ) : gs_param_t<val_type>(string(nam), false, NULL, true) { init(); } 

  explicit gs_param(const std::string &nam, const std::string &val, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { init(); } 
  explicit gs_param(const char *nam,        const char *val       , const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { init(); } 
  explicit gs_param(const std::string &nam, const char *val       , const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { init(); } 
  explicit gs_param(const char *nam,        const std::string &val, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { init(); } 
  
  explicit gs_param(const std::string &nam, const val_type &val, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, NULL, true )  { init(); }    
  explicit gs_param(const char *nam,        const val_type &val, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, NULL, true )  { init(); }    
  
  explicit gs_param(                                                gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(             force_top_level_name, parent_array, true) { init(); } 
  explicit gs_param(const std::string &nam                        , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { init(); } 
  explicit gs_param(const char *nam                               , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { init(); } 
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { init(); } 
  explicit gs_param(const char *nam,        const char *val       , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { init(); } 
  explicit gs_param(const std::string &nam, const char *val       , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { init(); } 
  explicit gs_param(const char *nam,        const std::string &val, gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { init(); } 
  explicit gs_param(const std::string &nam, const val_type &val   , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { init(); } 
  explicit gs_param(const char *nam,        const val_type &val   , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { init(); } 
  
  explicit gs_param(                                                gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(             force_top_level_name, &parent_array, true) { init(); } 
  explicit gs_param(const std::string &nam                        , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { init(); } 
  explicit gs_param(const char *nam                               , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { init(); } 
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { init(); } 
  explicit gs_param(const char *nam,        const char *val       , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { init(); } 
  explicit gs_param(const std::string &nam, const char *val       , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { init(); } 
  explicit gs_param(const char *nam,        const std::string &val, gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { init(); } 
  explicit gs_param(const std::string &nam, const val_type &val   , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { init(); } 
  explicit gs_param(const char *nam,        const val_type &val   , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { init(); } 
  
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_t<val_type>(nam, force_top_level_name,  parent_array, register_at_db) { init(); }
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_t<val_type>(nam, force_top_level_name, &parent_array, register_at_db) { init(); } 
  
  virtual ~gs_param() { gs_param_t<val_type>::destruct_gs_param(); }              
                                                                                  
  bool deserialize(val_type &target_val, const std::string& str) {
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    if (gs_param_base::m_locked) {                                                
      GS_PARAM_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;                                                               
    }                           
#endif
    return static_deserialize(target_val, str);                                   
  }                                                                               
                                                                                  
private:
  my_type& operator = (const my_type& v) { 
    return *this;
  }

public:                                                                                   
  using gs_param_t<val_type>::name;                                
  using gs_param_t<val_type>::setString;                                          
  using gs_param_t<val_type>::getString;                                          
  using gs_param_t<val_type>::setValue;                                           
  using gs_param_t<val_type>::getValue;
 
  bool lock(void* pwd = NULL) {
    SC_REPORT_WARNING(name(), "Lock not supported for sc_event parameter type!");
    return false;
  }
  bool unlock(const void* pwd = NULL) {
    SC_REPORT_WARNING(name(), "Lock not supported for sc_event parameter type!");
    return true;
  }
    
  // eventthread is sensitive to intern_event; makes callback and notify my_value event
  void eventthread(void) {
      make_post_write_callbacks();
      my_value.notify();
  }

  void init(){
    GS_PARAM_DUMP("Init gs_param_t "<< m_par_name.c_str());
    
    sc_core::sc_spawn_options opt;
    opt.dont_initialize();
    opt.set_sensitivity(&intern_event);
    sc_core::sc_spawn(sc_bind(&gs::cnf::gs_param<sc_core::sc_event>::eventthread,this), sc_core::sc_gen_unique_name("gsparam_scevent_internevent"), &opt);

    // add to plugin database
    if (m_register_at_db) {
      m_api->addPar(this);
    }
  }  

  void notify() {
    intern_event.notify();
  }
  void notify(const sc_time& t) {
    intern_event.notify(t);
  }
  void notify(double d , sc_time_unit t) {
    intern_event.notify(d, t);
  }
  void cancel() {
    intern_event.cancel();
    my_value.cancel();
  }

  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("sc_event");
  }
  
  /// Overloads gs_param_base::getType
  Param_type getType() const {
    return PARTYPE_SC_EVENT;
  }
  
  /// serialize(sc_logic)
  std::string serialize(const sc_core::sc_event &v) {
    GS_PARAM_DUMP_WITHNAME("gs_param<sc_event>", "serialize: sc_event has no value!");
    return std::string("");
  }
  
  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    return static_serialize(val);
  }  
  inline static std::string static_serialize(const val_type &val) {
    GS_PARAM_DUMP_WITHNAME("gs_param<sc_event>", "serialize: sc_event has no value!");
    return std::string("");
  }
    
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    GS_PARAM_DUMP_WITHNAME("gs_param<sc_event>", "static_deserialize: sc_event has no value!");
    return false;
  }
};

