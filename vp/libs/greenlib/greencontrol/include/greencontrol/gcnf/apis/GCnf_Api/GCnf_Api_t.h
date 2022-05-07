//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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

// doxygen comments

#ifndef __GCNF_API_T_H__
#define __GCNF_API_T_H__

#include <map>

#include "boost/shared_ptr.hpp" 

#include "greencontrol/gcnf/plugin/config_globals.h"
#include "greencontrol/core/gc_port_if.h"
#include "greencontrol/core/gc_port.h"
#include "greencontrol/core/initialize_if.h"
#include "greencontrol/core/command_if.h"
#include "greencontrol/core/gc_transaction.h"
#include "greencontrol/gcnf/plugin/gcnf_datatypes.h"  // Command enumeration
#include "greencontrol/gcnf/plugin/config_callbadapt.h"
#include "greencontrol/gcnf/plugin/config_paramcallbadapt.h"
#include "greencontrol/gcnf/plugin/utils.h"
#include "greencontrol/gcnf/plugin/string_vector.h"
#include "cnf_api_if.h"

#define GCNF_API_NAME "GCnf_Api"
#undef DUMMY_NAME
#define DUMMY_NAME "**dummy.NewParamObservers**"


namespace gs {
namespace cnf {


// forward declaration 
class gs_param_base;

template<class T> 
class gs_param;

template<typename gs_param_base_T,  template<class T> class gs_param_T, typename ConfigPlugin_T>
class GCnf_private_Api_T;
  
/// GreenControl Config API (typedefed to GCnf_Api) to be used by other APIs.
/**
 * This is the GreenControl Config API which may be used by other APIs to
 * access the GreenControl Core.
 *
 * This class (GCnf_Api_t) is templated to resolve cyclic includes with
 * gs_param_base. This class _only_ works with gs_param_base as template
 * parameter! (See typedef GCnf_Api.)
 *
 * This API may be used even before initialize-mode:
 *
 * All notifies are made without events and time.
 */
template<typename gs_param_base_T, typename GCnf_private_Api_TMPL, template<typename T> class gs_param_T, typename ConfigPlugin_T>
class GCnf_Api_t
: public sc_object
, public initialize_if
, public cnf_api_if
, public gc_port_if
, public command_if
{

  friend class GCnf_private_Api_T<gs_param_base_T, gs_param_T, ConfigPlugin_T>;
  
  friend class ConfigPlugin;
  
  /// Typedef to the API type
  typedef GCnf_Api_t<gs_param_base_T, GCnf_private_Api_TMPL, gs_param_T, ConfigPlugin_T> GCnf_Api;
  
  // Typedef for static vector containing pointers to the GCnf_Api
  // typedef std::vector<GCnf_Api*>  ApiVector;

  // Typedef for map containing pointers to the GCnf_Api and its module
  typedef std::map<sc_module *, cnf_api_if *>   ModuleApiMap;

  // Typedef for static map containing pointers to the _private_ GCnf_private_Apis
  //               name-string, private API pointer
  typedef std::map<std::string, GCnf_private_Api_TMPL* >  PrivApiMap;
  
  /// Typedef for a multimap saving pointers to callback functions for each parameter.
  /**
   * The multimap may contain several callback pointers for each parameter.
   *
   * The map saves CallbAdapt_b objects which contain the object pointer and the
   * member function pointer.
   */
  typedef std::multimap<std::string, CallbAdapt_b*> observerCallbackMap;

public:

  /// Static function to get a GCnf_Api or GCnf_private_Api instance (default or private).
  /**
   * Function to get an instance of the GCnf_Api which is the default 
   * one or a private one.
   * 
   * This method can be used either to
   * - get the default API instance (normal case) or to
   * - get an existing private API instance instead or to
   * - create the default API instance (mod = NULL) (called by the plugin)
   *
   * Prefered coding style: Get a GCnf_Api by calling this 
   * static function (only) _once_ per module / object and storing
   * the returned pointer for later accesses (because this 
   * function is not optimized for efficency)!
   *
   * This function should be called with the module pointer of the
   * caller (or its parent sc_core::sc_module if it is not a module itself)
   * because the test for private APIs is done with the pointer.
   * Only at toplevel it is allowed to call this function with 
   * mod=NULL.
   *
   * @todo getApiInstance shall take an sc_object or a string, not needing an sc_module!
   *
   * Caution: APIs will be destroyed when the owner destroys it 
   *
   * @param mod  User module pointer which identifies the needed API instance.
   * @return     Pointer to the API instance.
   */ 
  static cnf_api_if* getApiInstance(sc_core::sc_module *mod) 
  {
    // ensure the plugin is existing
    ConfigPlugin_T::get_instance();

    if (!mod) {
      GCNF_DUMP_N("static GCnf_Api::getApiInstance", "No pointer address to parent module, use/create default GCnf_Api.");
      // TODO: Kann man testen, ob der caller ein module ist? Dann naemlich DEPRECATED WARNING weil nicht auf private API ueberprüft werden kann
      //       wenn test nicht moeglich, SC_INFO ausgeben. (Fuer toplevel APIs ist der call mit NULL erlaubt.)
      create_default_instance();
    }
    else {
      GCNF_DUMP_N("static GCnf_Api::getApiInstance", "module pointer: "<<(void*)mod<<" and name: "<<mod->name());
    }
    
    return handleApiInstances(mod, NULL);
  }
  
  /// Static function to create a private GCnf_API.
  /**
   * Creates a private GCnf_Api instance (class GCnf_private_Api)
   * which registers the instance.
   *
   * Cannot set not-private parameters. Create an private 
   * API directly instead.
   *
   * The caller must take care to delete the returned instance
   * after all child modules have been destructed!
   *
   * @param owner_module  Module who creates and owns the private API.
   * @return  Pointer to the new created private API.
   */
  static GCnf_private_Api_TMPL* create_private_ApiInstance(const sc_core::sc_module* owner_module) {
    assert(owner_module); // TODO: SC_REPORT_WARNING
    return new GCnf_private_Api_TMPL(owner_module);
  }

protected:

  /// Guarded function being called by the ConfigPlugin constructor
  /*
   * This function guards the problem of cyclic creations:
   * default Api -> creates Plugin -> creates default Api
   */
  static void create_default_instance() {
    static bool currently_creating_default_instance = false;
    if (!currently_creating_default_instance) {
      currently_creating_default_instance = true;
      handleApiInstances(NULL, NULL);
    }
    currently_creating_default_instance = false;
  }
  
  /*
   * Why may we support shared pointers here? To cover the unlikely
   * case that an owner module is destructed while other modules
   * are still using the API. (Should not happen since only childs
   * of the owning module should use the private API.)
   *  Issue: during construction a priv API cannot add itself with 
   *         a shared pointer!
   */
  
  /// Static function to register a private GCnf_API (contained in a shared pointer).
  /**
   * Register a private GCnf_Api instance (class GCnf_private_Api)
   * to make it available to be returned in the getApiInstance 
   * function.
   *
   * This function does not get a shared pointer?
   * This should be no problem since only childs of the owning 
   * module should use the private API. Accordingly the API will
   * be living at least as long as the childs do (as long as the
   * API is a member of the owning module).
   *
   * @param privApi  Pointer to the private API. Must be a private API of type gs::cnf::GCnf_private_Api !
   * @return If creation was successfull (if there was not already an existing API for this name).
   */
  static bool register_private_ApiInstance(GCnf_private_Api_TMPL* privApi) {
    if (handleApiInstances(NULL, privApi)) return true;
    else return false;        
  }
  
  /// Static function to handle (register and get) GCnf_[private_]Api instances (default or private).
  /**
   * This method can be used either to
   * - get the default API instance (normal case) or to
   * - get an existing private API instance instead or to
   * - create the default API instance (mod = NULL) (called by the plugin)
   * - register private API instances
   *
   * @param mod_to_get  User module pointer which identifies the needed API instance.
   * @param privApi_to_register  Pointer to private API that should be registered.
   * @return     Pointer to the API instance.
   */ 
  static cnf_api_if* handleApiInstances(sc_core::sc_module *mod_to_get, 
                                     GCnf_private_Api_TMPL* privApi_to_register) 
  {
    // ensure the plugin is existing
    ConfigPlugin_T::get_instance();

    // ********* Preparing work *************
    ModuleApiMap::iterator it;
    static PrivApiMap* mPrivInstanceMap = NULL;
    static GCnf_Api *mDefApi = NULL;
    /// Map to save the Api returned corresponding to each module
    static ModuleApiMap m_module_api_map;
    cnf_api_if* gcapi = NULL;

    if (!initialize_mode && !mDefApi) { // Check if the default is not already after ene_of_elab 
      SC_REPORT_ERROR("static GCnf_Api::handleApiInstance", "The default GCnf_Api has not been created before end_of_elaboration! You have to instantiate the ConfigPlugin first (or call at least once this function during elaboration)!");
      return NULL;
    }
    if (!mDefApi) {
      GCNF_DUMP_N("static GCnf_Api::handleApiInstance", "create new default API");
      mDefApi = new GCnf_Api_t(true);
    }
  
    if (!mPrivInstanceMap) {
      GCNF_DUMP_N("static GCnf_Api::handleApiInstance", "create new PrivApiMap");
      mPrivInstanceMap = new PrivApiMap();
    } // TODO: delete this instance!?
    
    // ********* Registering private APIs *************
    if (privApi_to_register) {
      GCNF_DUMP_N("static GCnf_Api::register_private_ApiInstance", "Register private API for owner module '"<<privApi_to_register->get_owner_name().c_str()<<"'");
      std::pair<typename PrivApiMap::iterator, bool> ret;
      // Add this new API to the map
      ret = mPrivInstanceMap->insert(make_pair(privApi_to_register->get_owner_name(), privApi_to_register));
      // Check for a already existing private API for this owner
      if (!ret.second) {
        GCNF_DUMP_N("static GCnf_Api::register_private_ApiInstance", "Failed! There is already existing a private API for this owner!");
        SC_REPORT_ERROR("static GCnf_Api::register_private_ApiInstance", "Failed! There is already existing a private API for this owner!");
        return gcapi; // = false
      }
      return privApi_to_register; // = true
    }

    if(mod_to_get)
    {
        // If for the overgiven module <mod> (or a parent) a private API exists, return that
        typename PrivApiMap::iterator pos;
        const std::string mod_name = mod_to_get->name();
        std::size_t search_length = mod_name.length();
        //cout << "search for private APIs for module '"<<mod_name<<"'" << endl;
        // try with the hierarchical name - shortened at each iteration - until no hierarchy is left
        //  finds the private API with the longest name match!
        while(search_length > 0 && search_length != string::npos) 
        {
          if (gcapi) break;
          //cout << "  search for API responsible for module '"<< mod_name.substr(0, search_length) <<"'" << endl;
          for (pos = mPrivInstanceMap->begin(); pos != mPrivInstanceMap->end(); pos++) {
            //cout << "    try '"<< pos->first <<"'"<<endl;
            if (pos->first.compare(mod_name.substr(0,search_length)) == 0 ) {
              gcapi = pos->second;
              //cout << "  found private API '"<<pos->first<<"'!" << endl;
              GCNF_DUMP_N("static GCnf_Api::getApiInstance", "Return private API '"<<pos->first.c_str()<<"'.");
              break;
            }
           }
          // cut off last hierarchy of mod_name (for find call)
          search_length = mod_name.find_last_of(SC_NAME_DELIMITER, search_length-1);
        }
        if (gcapi) {
           return gcapi;
        }
    }
    else
    {
       // no module, return the default API
       GCNF_DUMP_N("static GCnf_Api::getApiInstance", "Return default API.");
       return mDefApi;
    }

    // if no private API for this module, find if there is already gcapi 
    // registered for this module
    it = m_module_api_map.find(mod_to_get);
    if (it != m_module_api_map.end())
    {
       return it->second;
    }
    else {
      // TODO: remove this limitation by not using SystemC ports that can only be bound before simulation
      // see task http://www.greensocs.com/en/node/2152/
      if (initialize_mode) {
        gcapi = new GCnf_Api_t(mod_to_get);
        m_module_api_map.insert(std::pair<sc_module *, cnf_api_if *>(mod_to_get,gcapi));
        return gcapi;
      }
      else {
        GCNF_DUMP_N("static GCnf_Api::getApiInstance", "Cannot new API during simulation: return default API.");
        return mDefApi;
      }
    }
  }

  // Deprecated workaround: for the organization of callbacks calling the deprecated callbacks
  //GC_HAS_CALLBACKS();
  std::map<void*, boost::shared_ptr< ::gs::cnf::ParamCallbAdapt_b> > gcnf_ParamCallbAdapt_list;
  boost::shared_ptr< ::gs::cnf::ParamCallbAdapt_b> gc_add_ParamCallbAdapt(boost::shared_ptr< ::gs::cnf::ParamCallbAdapt_b> cba) {
    gcnf_ParamCallbAdapt_list.insert(std::pair<void*, boost::shared_ptr< ::gs::cnf::ParamCallbAdapt_b> >((cba.get()), cba));             
    return cba;
  }
  
public:
  
  /// DEPRECATED Constructor.
  /**
   * Recommended coding style to get a GCnf_Api instance is
   * to call the static function gs::cnf::getApiInstance(sc_core::sc_module*) 
   * with a valid sc_core::sc_module pointer.
   */
  GCnf_Api_t()
  : sc_object(sc_gen_unique_name("__gcnf_api__"))
  , m_gc_port(CONFIG_SERVICE, GCNF_API_NAME, false, this)
  , m_registered_as_new_param_observer(false)
  , m_new_param_event(NULL)
  {     
    GCNF_DUMP_N(name(), "constructor GCnf_Api()");
    
    // ensure the plugin is existing
    ConfigPlugin_T::get_instance();

    // add this Api instance to the static ApiMap to allow parameters to use it!
    //getApiInstance(get_parent_sc_module(this), this);
    DEPRECATED_WARNING(name(), "GCnf_Api() is deprecated! Use the static gs::cnf::getApiInstance(sc_core::sc_module*) instead!");
    if (check_for_private_Api()) {
      SC_REPORT_FATAL(name(), "There is existing a private config API for a module that wants to instantiate a not-private API! Use GCnf_Api::getApiInstance(sc_core::sc_module*) instead!");
      assert(false);
    }
  }

  /// DEPRECATED Constructor for other APIs which use the GCnf_Api.
  /**
   * Recommended coding style to get a GCnf_Api instance is
   * to call the static function gs::cnf::getApiInstance(sc_core::sc_module*) 
   * with a valid sc_core::sc_module pointer.
   *
   * This constructor was formerly used by other APIs which
   * set their own API name.
   * TODO: check if really deprecated or maybe not
   */
  explicit GCnf_Api_t(const char* api_name)
  : sc_object(sc_gen_unique_name("__gcnf_api__"))
  , m_gc_port(CONFIG_SERVICE, api_name, false, this)
  , m_registered_as_new_param_observer(false)
  , m_new_param_event(NULL)
  { 
    GCNF_DUMP_N(name(), "constructor GCnf_Api("<<api_name<<")");
    
    // ensure the plugin is existing
    ConfigPlugin_T::get_instance();

    // add this Api instance to the static ApiMap to allow parameters to use it!
    //getApiInstance(get_parent_sc_module(this), this);
    DEPRECATED_WARNING(name(), "GCnf_Api(name) is deprecated! Use the static gs::cnf::getApiInstance(sc_core::sc_module*) instead!");
    if (check_for_private_Api()) {
      SC_REPORT_FATAL(name(), "There is existing a private config API for a module that wants to instantiate a not-private API! Use GCnf_Api::getApiInstance(sc_core::sc_module*) instead!");
      assert(false);
    }
  }

  // Constructor to be called by static function getApiInstance
  /**
   * The is the only constructor that should be called.
   * It should only be called by the static function 
   * gs::cnf::getApiInstance.
   */
  explicit GCnf_Api_t(const bool called_by_static)
  : sc_object(sc_gen_unique_name("__gcnf_api__"))
  , m_gc_port(CONFIG_SERVICE, GCNF_API_NAME, false, this)
  , m_registered_as_new_param_observer(false)
  , m_new_param_event(NULL)
  { 
    if (!called_by_static) {
      SC_REPORT_ERROR(name(), "This constructor should only be called by the static function gs::cnf::getApiInstance!");
    }
    assert(called_by_static);
    GCNF_DUMP_N(name(), "constructor GCnf_Api(true)");

    // ensure the plugin is existing
    ConfigPlugin_T::get_instance();
  }
  
  /// Destructor
  ~GCnf_Api_t() {

    if (m_new_param_event != NULL) 
      delete m_new_param_event;

    // Deprecated workaround: for the organization of callbacks calling the deprecated callbacks
    //GC_UNREGISTER_CALLBACKS();
    for (std::map<void*, boost::shared_ptr< ::gs::cnf::ParamCallbAdapt_b> >::iterator iter = gcnf_ParamCallbAdapt_list.begin(); 
         iter != gcnf_ParamCallbAdapt_list.end(); iter++) {   
    (*iter).second->unregister_at_parameter(); }            
    gcnf_ParamCallbAdapt_list.clear();
    
    // TODO: within try-catch?
    // Delete all CallbAdapt_b in m_observer_callback_map
    CallbAdapt_b *cba;
    for (observerCallbackMap::iterator iter = m_observer_callback_map.begin(); iter!=m_observer_callback_map.end(); ++iter) {
      cba = iter->second;
      delete cba;
      m_observer_callback_map.erase(iter);
    }
    // TODO: If here occurs a segfault, this may be because of the ObjectFactory<ControlTransaction> that
    //       is destructed in the API's master port. Pushing down the pool size removed this error last time.
  }
  

  ////////////////////// GC_PORT_IF ///////////////////////////////////

  /// Called by gc_port through gc_port_if when notification arrives.
  /**
   * Implements gc_port_if.
   * This method starts whenever a master triggers a payload-event.
   */
  void transport(ControlTransactionHandle &tr)
  {
    
    GCNF_DUMP_N(name(), "got transaction atom from master");      

    // show received Transaction
    GCNF_DUMP_N(name(), "  received transaction: "<<(tr->toString()).c_str());      

    // According to the command fill the transaction or make actions
    switch (tr->get_mCmd()) {

    case CMD_NOTIFY_NEW_PARAM_OBSERVER:
      {
        const std::string dummyParName = DUMMY_NAME;

        // Notify the oberserver events
        GCNF_DUMP_N(name(), "transports: notify event for new added parameter "<<tr->get_mSpecifier().c_str());      
        
        if (!m_registered_as_new_param_observer) {
          GCNF_DUMP_N(name(), "transport: error: no event for new parameters");
          tr->set_mError(1);
        } else {
          if (m_new_param_event != NULL) {
            // Attention: Only wait for these events after elaboration! (Before use callbacks.)
            // notify event
            m_new_param_event->notify();
          }

          // Call back all the callback observer methods for this parameter
          if (tr->get_mLogPointer() != NULL)
            makeCallbacks(dummyParName, 
                          static_cast<gs_param_base_T*>(tr->get_mLogPointer())->getName(), 
                          static_cast<gs_param_base_T*>(tr->get_mLogPointer())->getString()  );
          else
            makeCallbacks(dummyParName, tr->get_mSpecifier(), tr->get_mValue() );
        }
       
        break;
      }

    default:
      SC_REPORT_WARNING(name(), "Unknown command in transaction");
    }
  }

  // /////////////////    GC-API methods   ////////////////////////////////////////// //

  /// Add a parameter without initial value to the ConfigPlugin. Does not work with new design. DEPRECATED!
  /**
   * A gs_param instance will be created at the plugin.
   * No meta_data is passed to this function as by adding another string param
   * will create ambiguity with other function. So, in case you want to pass  meta data
   * use another version of this function.
   */
  bool addParam(const std::string &parname) {
    DEPRECATED_WARNING(name(), "DEPRECATED: addParam is deprecated due to simulation overhead! Use 'setInitValue(\"par_name\", \"value\")' instead or create a gs_param<type> object.");
    GCNF_DUMP_N(name(), "addParam("<<parname.c_str()<<"), deprecated");
    return addParam(parname, string(""));
  }

  /// Add a parameter with initial value to the ConfigPlugin. DEPRECATED!
  /**
   * A gs_param instance will be created at the plugin.
   */
  bool addParam(const std::string &parname, const std::string &default_val, std::string meta_data = "") {
    DEPRECATED_WARNING(name(), "DEPRECATED: addParam is deprecated due to simulation overhead! Use 'setInitValue(\"par_name\", \"value\")' instead or create a gs_param<type> object.");
    GCNF_DUMP_N(name(), "addParam("<<parname.c_str()<<", "<<default_val.c_str()<<"), deprecated");
    if (!isHierarchicalParameterName(parname)) {
      SC_REPORT_INFO(name(), "Parameter names should be hierarchical names (expect this is a top-level parameter)!");
    }

    // Create Transaction and send it to config plugin
    ControlTransactionHandle th = m_gc_port.createTransaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_ADD_PARAM);
    //th->set_mAnyPointer(par);
    th->set_mSpecifier(parname); // e.g. "TestIP1.Param1"
    th->set_mValue(default_val);  // e.g. "TestValue1"
    if (meta_data != "") {
      GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
      th->set_mMetaData(meta_data);
    }
    
    // Different behavior during initilize-mode: use immediate call
    m_gc_port->transport(th);
    if (th->get_mError() == 0) {
      GCNF_DUMP_N(name(), "addParam: ... adding of param "<<parname.c_str()<<" successfull");
    } else {
      GCNF_DUMP_N(name(), "addParam: ... adding of param "<<parname.c_str()<<" failed (error "<<th->get_mError()<<")!");
      SC_REPORT_WARNING(name(), "addParam failed!");
      return false;
    }
    return true;
  }
    
  /// Add a parameter to the ConfigPlugin.
  /** 
   * Note: addPar (and all related methods) must not call any of the 
   *       pure virtual functions in gs_param_base because this method is 
   *       called by the gs_param_base constructor.
   *
   * @param par Parameter (including name and value).
   * @return Success of the adding.
   */
  bool addPar(gs_param_base_T* par, std::string meta_data = "") {
    if (par == NULL) {
      SC_REPORT_WARNING(name(), "addPar: Parameter is NULL!");
      return false;
    }
    GCNF_DUMP_N(name(), "addPar(par) name="<<par->getName().c_str());

    // Create Transaction and send it to config plugin
    ControlTransactionHandle th = m_gc_port.createTransaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_ADD_PARAM);
    th->set_mLogPointer(par);
    //th->set_mSpecifier(parname); // e.g. "TestIP1.Param1"
    //th->set_mValue(init_val);  // e.g. "TestValue1"
    if (meta_data != "") {
      GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
      th->set_mMetaData(meta_data);
    }
    
    // Different behavior during initilize-mode: use immediate call
    m_gc_port->transport(th);
    if (th->get_mError() == 0) {
      GCNF_DUMP_N(name(), "addPar: ... adding of param "<<par->getName().c_str()<<" successfull");
    } else {
      GCNF_DUMP_N(name(), "addPar: ... adding of param "<<par->getName().c_str()<<" failed (error "<<th->get_mError()<<")!");
      SC_REPORT_WARNING(name(), "addParam failed!");
      return false;
    }
    return true;
  }

  /// Remove a parameter from the ConfigPlugin. Called by the parameter destructor.
  /**
   * Checks if destruction flag is set (so this may only be called by the
   * parameter destructor).
   *
   * @param par Parameter pointer.
   * @return Success of remove.
   */
  bool removePar(gs_param_base_T* par, std::string meta_data = "") {
    if (par == NULL) SC_REPORT_ERROR(name(), "removePar: Parameter is NULL!");
    GCNF_DUMP_N(name(), "removePar(par) name="<<par->getName().c_str());
    
    if (!par->is_destructing()) {
      GCNF_DUMP_N(name(), "removePar: This method may only be called by the parameter's destructor itself");
      SC_REPORT_ERROR(name(), "removePar: This method may only be called by the parameter's destructor itself.");
      sc_assert(false);
      return false;
    }
    
    // Do not send transaction if simulation has already stopped
    // (because some GreenConfig Core objects may already be destructed)
    // TODO!!!!
    //if (!sc_is_running()/*sc_end_of_simulation_invoked()*/) {
    //  GCNF_DUMP_N(name(), "removePar: Removing will NOT be performed after simulation (because some GreenConfig Core objects may already be destructed).");
    //  return false;
    //}
    
    // Create Transaction and send it to config plugin
    ControlTransactionHandle th = m_gc_port.createTransaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_REMOVE_PARAM);
    th->set_mLogPointer(par);
    if (meta_data != "") {
      GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
      th->set_mMetaData(meta_data);
    }
    m_gc_port->transport(th);

    // handle returned errors
    if (th->get_mError() == 0) {
      GCNF_DUMP_N(name(), "removePar: ... remove of param "<<par->getName().c_str()<<" successfull");
    } else {
      GCNF_DUMP_N(name(), "removePar: ... remove of param "<<par->getName().c_str()<<" failed (error "<<th->get_mError()<<")!");
      SC_REPORT_ERROR(name(), "removeParam failed!");
      return false;
    }
    return true;
  }
  
  /// Set a parameter's value. Deprecated!
  /**
   * @param parname Hierarchical parameter name.
   * @param value   Value the parameter has to be set to.
   * @return        Success of the setting.
   */
  bool setParam(const std::string &parname, const std::string &value, std::string meta_data = "") {
    DEPRECATED_WARNING(name(), "DEPRECATED: setParam is deprecated due to simulation overhead! Use 'getPar(\"par_name\") = value' or 'setInitValue(\"par_name\", \"init_value\")' instead.");
    GCNF_DUMP_N(name(), "setParam("<<parname.c_str()<<", "<<value.c_str()<<"), deprecated");      
    gs_param_base_T* p = getPar(parname, meta_data);
    bool succ = false;
    if (p == NULL)   succ = setInitValue(parname, value, meta_data); // Add new parameter
    else succ = p->setString(value); // set existing parameter
    return succ;
  }

  /// Set a parameter's init value. 
  /**
   * The init value has priority to the initial value set by the owner!
   *
   * @param parname Hierarchical parameter name.
   * @param value   Init value the parameter has to be set to.
   * @return        Success of the setting.
   */
  bool setInitValue(const std::string &parname, const std::string &value, std::string meta_data = "") {
    GCNF_DUMP_N(name(), "setInitValue("<<parname.c_str()<<", "<<value.c_str()<<")");      

    {
      // Create Transaction and send it to config plugin
      ControlTransactionHandle th = m_gc_port.createTransaction();
      th->set_mService(CONFIG_SERVICE);
      th->set_mCmd(CMD_SET_INIT_VAL);
      th->set_mSpecifier(parname); // e.g. "TestIP1.Param1"
      th->set_mValue(value);  // e.g. "TestValue1"
      if (meta_data != "") {
        GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
        th->set_mMetaData(meta_data);
      }
      
      m_gc_port->transport(th);

      if (th->get_mError() == 0) {
        GCNF_DUMP_N(name(), "setInitValue: ... setting successfull");
      } else if (th->get_mError() == 2) {
        GCNF_DUMP_N(name(), "setInitValue: ... setting failed (error "<<th->get_mError()<<")!");
        SC_REPORT_WARNING(name(), "setInitValue: ... setting failed because init value is locked!");
        return false;
      } else {
        GCNF_DUMP_N(name(), "setInitValue: ... setting failed (error "<<th->get_mError()<<")!");
        SC_REPORT_WARNING(name(), "setInitValue: ... setting failed!");
        return false;
      }
    }
    return true;
  }
  
  bool lockInitValue(const std::string &parname, std::string meta_data = "") {
    GCNF_DUMP_N(name(), "lockInitValue("<<parname.c_str()<<")");      
    
    {
      // Create Transaction and send it to config plugin
      ControlTransactionHandle th = m_gc_port.createTransaction();
      th->set_mService(CONFIG_SERVICE);
      th->set_mCmd(CMD_LOCK_INIT_VAL);
      th->set_mSpecifier(parname); // e.g. "TestIP1.Param1"
      if (meta_data != "") {
        GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
        th->set_mMetaData(meta_data);
      }
      
      m_gc_port->transport(th);
      
      if (th->get_mError() == 0) {
        GCNF_DUMP_N(name(), "lockInitValue: ... locking successfull");
      } else {
        GCNF_DUMP_N(name(), "lockInitValue: ... locking failed (error "<<th->get_mError()<<")!");
        SC_REPORT_WARNING(name(), "lockInitValue: ... locking failed!");
        return false;
      }
    }
    return true;
  }


  /// Get a parameter's value. Deprecated!
  /**
   * @param   parname Hierarchical parameter name.
   * @return  Value (string representation) of the parameter <parname>.
   */ 
  const std::string& getParam(const std::string &parname, std::string meta_data = "") {
    DEPRECATED_WARNING(name(), "DEPRECATED: getParam is deprecated due to simulation overhead! Use 'value = getPar(\"par_name\")' or 'getPar(...)->getValue()' or 'getPar(...)->getString()' instead.");
    GCNF_DUMP_N(name(), "getParam("<<parname.c_str()<<"), deprecated");      
    gs_param_base_T *p = getPar(parname);
    if (p == NULL) {
      static std::string u = string();
      return u;
    }
    return p->getString();
  }
  
  /// @see gs::cnf::cnf_api_if::getValue
  const std::string getValue(const std::string &parname, std::string meta_data = "", const bool not_impact_is_used_status = false) {
    GCNF_DUMP_N(name(), "getValue("<<parname.c_str()<<", not_impact_is_used_status="<<not_impact_is_used_status<<")");      
    // create Transaction an send it to config plugin
    ControlTransactionHandle th = m_gc_port.createTransaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_GET_VAL);
    th->set_mSpecifier(parname); // e.g. "TestIP1.TestParam1"
    if (meta_data != "") {
      GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
      th->set_mMetaData(meta_data);
    }
    if (not_impact_is_used_status) {
      GCNF_DUMP_N(name(), "Setting mAnyPointer to bool not_impact_is_used_status");
      th->set_mAnyPointer((void*)&not_impact_is_used_status);
    }
    
    m_gc_port->transport(th);
    
    if (th->get_mError() == 0) {
      GCNF_DUMP_N(name(), "getValue: ... got value = "<<th->get_mValue().c_str());
    } else {
      GCNF_DUMP_N(name(), "getPar: ... getting failed (error "<<th->get_mError()<<")!");
      SC_REPORT_WARNING(name(), std::string("getPar: " + parname + " not found !").c_str());
    }
    
    return th->get_mValue();
  }

  /// @see gs::cnf::cnf_api_if::getPar
  gs_param_base_T* getPar(const std::string &parname, std::string meta_data = "") {
    GCNF_DUMP_N(name(), "getPar("<<parname.c_str()<<")");

    // create Transaction an send it to config plugin
    ControlTransactionHandle th = m_gc_port.createTransaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_GET_PARAM);
    th->set_mSpecifier(parname); // e.g. "TestIP1.TestParam1"
    if (meta_data != "") {
      GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
      th->set_mMetaData(meta_data);
    }
    
    m_gc_port->transport(th);
    
    gs_param_base_T *par = static_cast<gs_param_base_T*>(th->get_mLogPointer());
    if (th->get_mError() == 0) {
      GCNF_DUMP_N(name(), "getPar: ... got parameter");
    } else {
      GCNF_DUMP_N(name(), "getPar: ... getting failed (error "<<th->get_mError()<<")!");
      //SC_REPORT_INFO(name(), "getPar: ... getting failed!");
    }

    return par;
  }

  /// Checks whether a parameter exists (implicit or explicit).
  /**
   * @param parname  Hierarchical parameter name.
   * @return Whether the parameter <parname> exists in the ConfigPlugin.
   */
  bool existsParam(const std::string &parname, std::string meta_data = "") {
    GCNF_DUMP_N(name(), "existsParam("<<parname.c_str()<<")");      

    // create Transaction an send it to config plugin
    ControlTransactionHandle th = m_gc_port.createTransaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_EXISTS_PARAM);
    th->set_mSpecifier(parname); // e.g. "TestIP1.TestParam1"
    if (meta_data != "") {
      GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
      th->set_mMetaData(meta_data);
    }
    
    m_gc_port->transport(th);

    if (th->get_mError() == 0) {
      GCNF_DUMP_N(name(), "existsParam: ... yes");
    } else {
      GCNF_DUMP_N(name(), "existsParam: ... no");
      return false;
    }
    return true;
  }
  
  /// Returns if the parameter has ever been used.
  /**
   * A parameter has been used if there is/was either a parameter object
   * mapped to the initial value, or the initial value has ever been read
   * (from the broker/ConfigAPI). 
   * If there is no implicit/explicit parameter with this name this returns false.
   *
   * Note: exists_param, lock_init_value, set_init_value, get_param_list 
   *       shall not impact the is_used status.
   *
   * @param parname  Full hierarchical parameter name.
   * @return If the parameter is or has been used.
   */
  bool is_used(const std::string &parname, std::string meta_data = "") {
    GCNF_DUMP_N(name(), "is_used("<<parname.c_str()<<")");      
    
    // create Transaction an send it to config plugin
    ControlTransactionHandle th = m_gc_port.createTransaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_PARAM_HAS_BEEN_ACCESSED);
    th->set_mSpecifier(parname); // e.g. "TestIP1.TestParam1"
    if (meta_data != "") {
      GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
      th->set_mMetaData(meta_data);
    }
    
    m_gc_port->transport(th);
    
    if (th->get_mError() == 0) {
      GCNF_DUMP_N(name(), "is_used: ... yes");
    } else {
      GCNF_DUMP_N(name(), "is_used: ... no");
      return false;
    }
    return true;
  }
  
  /// If a parameter is explicit.
  /**
   * @param   parname   Hierarchical parameter name.
   * @return  If the parameter is explicit.
   */ 
  /*bool is_explicit(const std::string &parname) {
    GCNF_DUMP_N(name(), "is_explicit("<<parname.c_str()<<")");
    
    // create Transaction an send it to config plugin
    ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_GET_PARAM);
    th->set_mSpecifier(parname); // e.g. "TestIP1.TestParam1"
    
    ControlPhase p(ControlPhase::CONTROL_REQUEST);
    ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
    m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);
    
    gs_param_base_T *par = static_cast<gs_param_base_T*>(th->get_mAnyPointer());
    if (th->get_mError() == 0) {
      return true;
    }
    return false;
  }*/
  

  /// Return a list of all parameters existing in the ConfigPlugin.
  /**
   * @return Vector with hierarchical parameter names.
   * can not pass meta data string as this will ead to ambiguity with other functions
   */
  const std::vector<std::string> getParamList() {
    return getParamList("", false);
  }

  /// Return a list of all parameter names (implicit and explicit) of the specified module (Use '.*' to include the parameters of the childs!).
  /**
   * The returned list contains all parameters of the module
   * whose name is given, not including it's childs.
   * If module_name end with '.*' the childs are included!
   *
   * @param module_name Name of the module whose parameter list should be returned.
   * @return Vector with hierarchical parameter names.
   */
  const std::vector<std::string> getParamList(const std::string &module_name, std::string meta_data = "") {
    return getParamList(module_name, false, meta_data);
  }

  /// Return a list of all parameter names (implicit and explicit) of the specified module (and childs).
  /**
   * If including_childs: All parameters of the module including 
   *                      the parameters of it's child modules.
   *
   * If not including_childs: All direct parameters of the module.
   *
   * @param module_name       Name of the module whose parameter list should be returned.
   * @param including_childs  If the parameters of the child modules should be included.
   * @return Vector with hierarchical parameter names.
   */
  const std::vector<std::string> getParamList(const std::string &module_name, bool including_childs, std::string meta_data = "") {
    std::stringstream ss;
    GCNF_DUMP_N(name(), "getParamList(...)");      

    // create Transaction an send it to config plugin
    ControlTransactionHandle th = m_gc_port.createTransaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_GET_PARAM_LIST_VEC);
    if (meta_data != "") {
      GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
      th->set_mMetaData(meta_data);
    }

    ss << module_name;
    if (including_childs) ss << ".*";
    th->set_mSpecifier(ss.str()); // module

    m_gc_port->transport(th);

    std::vector<std::string> vec;    
    string_vector* v = static_cast<string_vector*>(th->get_mLogPointer());
    if (th->get_mError() != 0 || v == NULL) {
      GCNF_DUMP_N(name(), "getParamList: ... getting list failed (error "<<th->get_mError()<<" or cast failed)!");
      SC_REPORT_WARNING(name(), "getParamList: ... getting list failed!");
      return vec;
    }

    vec = *(v);
    delete v;
    /*std::string token = th->get_mValue();
    std::vector<std::string> vec;
    // Convert val to vector
    std::string::size_type idx;
    // test for commas
    while((idx = token.find_first_of(",")) != std::string::npos) {
      if (idx > 0) { // is a word
        vec.push_back(token.substr(0,idx));
      }    
      token = token.substr(idx+1);
    }
    if (token.substr(0,idx) != "") vec.push_back(token.substr(0,idx));*/

#ifdef GC_VERBOSE
    // Show vector
    vector<string>::iterator iter;
    std::stringstream ss_show;
    iter = vec.begin();
    while( iter != vec.end() ) {
      if (iter != vec.begin()) ss_show << ", ";
      ss_show << *iter;
      iter++;
    }
    GCNF_DUMP_N(name(), "getParamList: ... got list for '"<<ss.str().c_str()<<"': '"<<ss_show.str().c_str()<<"'");
#endif

    return vec;
  }

  /// Return a pointer list of all (explicit) parameters of the specified module. (Use '.*' to include the parameters of the childs!)
  /**
   * The returned list contains all parameters of the module
   * whose name is given, not including it's childs.
   * If module_name ends with '.*' the childs are included!
   *
   * @param module_name Name of the module whose parameter list should be returned.
   * @return Vector with base parameter pointers.
   */
  const std::vector<gs_param_base*> getParams(const std::string &module_name = "") {
    GCNF_DUMP_N(name(), "getParams("<<module_name.c_str()<<")");
    std::vector<gs_param_base*> return_vec;
    std::vector<std::string> vec = getParamList(module_name, false);
    std::vector<std::string>::iterator iter;
    gs_param_base *p;
    for (iter = vec.begin(); iter != vec.end(); iter++) {
      p = getPar(*iter);
      if (p != NULL)
        return_vec.push_back(p);
    }
    return return_vec;
  }

  /// Returns an event which is notified when the parameter value changes. Deprecated!
  /**
   * Waiting for the returned event is not allowed during initialize-mode
   * (because of elaboration time). Instead of waiting use the callback
   * functions which work even during elaboration time.
   *
   * Throws RegisterObeserverFailedException if registering observer failed!
   *
   * @param parname   Name of the observed parameter
   * @return          Event which is notified at parameter change.
   */
  //sc_event& getUpdateEvent(const std::string &parname)  throw (RegisterObserverFailedException) {
  sc_event& getUpdateEvent(const std::string &parname) {
    DEPRECATED_WARNING(name(), "DEPRECATED: getUpdateEvent is deprecated! Use 'my_param.getUpdateEvent()' or use callback 'REGISTER_PARAM_CALLBACK(&my_param, class, method)' instead.");
    GCNF_DUMP_N(name(), "getUpdateEvent("<<parname.c_str()<<")");      
    gs_param_base_T* p = getPar(parname);
    if (p == NULL) {
      throw RegisterObserverFailedException("Get update event failed: Parameter not existing.", parname);
    }
    return p->getUpdateEvent();
  }

  // Makros moved to cnf_api_if.h
  // Makro for registering callback functions (see method registerCallback). DEPRECATED
  /*#define REGISTER_CALLBACK(class, method, parname)                \
    registerCallback(parname, new gs::cnf::CallbAdapt< class >(this, &class::method));

  // Makro for registering new parameter callback functions (see method registerCallback).
  #define REGISTER_NEW_PARAM_CALLBACK(class, method)                \
    registerNewParamCallback(new gs::cnf::CallbAdapt< class >(this, &class::method));
  */
  /// Registers an observer callback function (with the signature of callback_func_ptr). Use the REGISTER_CALLBACK macro!, Deprecated!
  /**
   * This method is deprecated! Use REGISTER_PARAM_CALLBACK(&my_param, class, method)
   * instead!
   *
   * For each parameter several callbacks may be registered.
   *
   * The callback works even during initialize-mode (elaboration time).
   *
   * Inside the callback functions no waits are allowed!
   *
   * The user may register any methods as callback functions which have
   * the following signature:
   * \code
   * void method_name(const std::string parname, const std::string value);
   * \endcode
   *
   * Usage example:
   * \code
   * class MyIP_Class
   * : public sc_core::sc_module
   * {
   * public:
   *   // some code [...]
   *   
   *   GC_nfApi my_GCnf_Api;
   *
   *   // Example code to register callback function
   *   void main_action() {
   *     // some code, parameters etc...
   *     my_GCnf_Api.REGISTER_CALLBACK(MyIP_Class, config_callback, "IP2.stringPar");
   *   }
   *
   *   // Callback function with default signature.
   *   void config_callback(const std::string parname, const std::string value) {
   *     // some action
   *   }
   * };
   * \endcode
   *
   * @param parname    Name of the parameter for which the callback is registered.
   * @param callb      Pointer to the CallbAdapt_b object which contains the object pointer
   *                   and the member function pointer.
   * @return           Success of registering.
   *
   * <em>Macro REGISTER_CALLBACK</em>
   *
   * Creates new CallbAdapt object and registers it at registerCallback.
   *
   * Usage:
   * \code
   * my_Api.REGISTER_CALLBACK(class_name, callback_method_name, parameter_name)
   * \endcode
   * Example:
   * \code
   * my_Api.REGISTER_CALLBACK(MyIP,       config_callback,      "IP2.stringPar")
   * \endcode
   */
  bool registerCallback(const std::string &parname, CallbAdapt_b *callb) {
    DEPRECATED_WARNING(name(), "DEPRECATED: registerCallback (and macro REGISTER_CALLBACK) is deprecated due to simulation overhead! Use 'REGISTER_PARAM_CALLBACK(&my_param, class, method)' instead.");
    GCNF_DUMP_N(name(), "registerCallback("<<parname.c_str()<<", obj, func_ptr)");      

    // Debug: Check if already a callback registered
    int cnt = m_observer_callback_map.count(parname);
    if (cnt > 0) {
      GCNF_DUMP_N(name(), "registerCallback: There is/are already "<<cnt<<" registered callback function(s) for the parameter "<<parname.c_str()<<".");
    }
    // If not already registered ask for new registration in plugin with getUpdateEvent-method
    else {
      // deprecated
      // workaround:
      //    register callback at parameter 
      //    and let the parameter call the method legacy_callback_adapter_method
      //    which calls the original method / event
      //GC_REGISTER_PARAM_CALLBACK(getPar(parname), GCnf_Api_t, legacy_callback_adapter_method);
      getPar(parname)->registerParamCallback( gc_add_ParamCallbAdapt(boost::shared_ptr< ::gs::cnf::ParamCallbAdapt_b>(new ::gs::cnf::ParamCallbAdapt<GCnf_Api_t>(this, &GCnf_Api_t::legacy_callback_adapter_method, this, (getPar(parname))))) );
    }

    // Add object and function pointer to multimap
    m_observer_callback_map.insert( std::pair<std::string, CallbAdapt_b*>( string(parname), callb ) );
    
    return true;
  }

  /// Returns an event which is notified when a new parameter is added or set implicitely the first time.
  /**
   * Waiting for the returned event is not allowed during initialize-mode
   * (because of elaboration time). Instead of waiting use the callback
   * functions which work even during elaboration time.
   *
   * Throws RegisterObeserverFailedException if registering observer failed!
   *
   * Attention: Most times this method is of no use because adding of parameters is done during
   *            elaboration and meanwhile no events are possible. Use the registerNewParamCallback
   *            method instead!
   *
   * This method returns an event which is notified each time a parameter is added
   * (without being an implicit parameter before) to the Config Plugin or an
   * (implicit) parameters value is set the first time (without being added before).
   *
   * @return          Event which is notified when new parameter is added.
   */
  //sc_event& getNewParamEvent() throw (RegisterObserverFailedException) {
  sc_event& getNewParamEvent() {
    DEPRECATED_WARNING("GCnf_Api", "DEPRECATED: The getNewParamEvent function is deprecated due to performance reasons! Register a callback instead using mApi.REGISTER_NEW_PARAM_CALLBACK(class_name, callback_function);");
    register_as_new_param_observer(); // automatically checks if already done
    if (m_new_param_event == NULL) {
      m_new_param_event = new sc_event();
    }
    return *m_new_param_event;
  }

  /// @see cnf_api_if::registerNewParamCallback
  bool registerNewParamCallback(CallbAdapt_b *callb) {
    const std::string parname = DUMMY_NAME;
    GCNF_DUMP_N(name(), "registerNewParamCallback(func_ptr)"); 

    // Debug: Check if already a callback registered
    int cnt = m_observer_callback_map.count(parname);
    if (cnt > 0) {
      GCNF_DUMP_N(name(), "registerCallback: There is/are already "<<cnt<<" registered callback function(s) for the parameter "<<parname.c_str()<<".");
    }

    register_as_new_param_observer(); // automatically checks if already done
    
    // Add object and function pointer to multimap
    m_observer_callback_map.insert( std::pair<std::string, CallbAdapt_b*>( string(parname), callb ) );
    
    return true;
  }

  /// Internal callback function that is called by the parameters. Deprecated!
  /** 
   * Registering is done in the methods getUpdateEvent (deprecated part) and registerCallback (deprecated)
   *
   * Does not perform the callback if the parameter is during destruction.
   * The legacy target module does not await this information and it 
   * cannot be transfered by the string value attribute.
   */
  void legacy_callback_adapter_method(gs_param_base_T &par) {
    GCNF_DUMP_N(name(), "legacy_callback_adapter_method: callback for parameter "<<par.getName().c_str());
    // only perform the callback if this parameter is NOT during destruction!!
    if (!par.is_destructing()) {
      // Call back all the callback observer methods for this parameter
      makeCallbacks(par.getName().c_str(), par.getName().c_str(), par.getString());
    }
  }


// replaced by GC_UNREGISTER_CALLBACKS(); in gs_param_base.h
// Macro to unregister all callbacks of this observer module. To be called in the user module's destructor. The parameter observersApi has to be the module's API.
//#define UNREGISTER_CALLBACKS(observersApi) observersApi.unregisterAllCallbacks(this)
  
  /// Unregisters all callbacks (within all existing parameters) for the specified observer module. 
  /**
   * Currently not used.
   *
   * Could be used by a observer's destructor instead of the 
   * macro <code>GC_UNREGISTER_CALLBACKS();</code>.
   *
   * @param observer   Pointer to the observer module who did register parameter callbacks.
   */
  void unregisterAllCallbacks(void* observer, std::string meta_data = "") {
    if (observer == NULL) {
      SC_REPORT_WARNING(name(), "unregisterAllCallbacks: Observer pointer is NULL!");
      return;
    }
    GCNF_DUMP_N(name(), "unregisterAllCallbacks(observer="<<observer<<")");
    
    // Do not send transaction if simulation has already stopped
    // (because some GreenConfig Core objects may already be destructed)
    // TODO!!!!
    //if (!sc_is_running()/*sc_end_of_simulation_invoked()*/) {
    //  GCNF_DUMP_N(name(), "removePar: Removing will NOT be performed after simulation (because some GreenConfig Core objects may already be destructed).");
    //  return false;
    //}
    
    // Create Transaction and send it to config plugin
    ControlTransactionHandle th = m_gc_port.createTransaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_UNREGISTER_PARAM_CALLBACKS);
    th->set_mAnyPointer(observer);
    if (meta_data != "") {
      GCNF_DUMP_N(name(), "Setting Meta Data with the value "<<meta_data.c_str());
      th->set_mMetaData(meta_data);
    }
    m_gc_port->transport(th);
    
    // handle returned errors
    if (th->get_mError() == 0) {
      GCNF_DUMP_N(name(), "unregisterAllCallbacks: ... unregister callbacks for obserserver "<<observer<<" successfull");
    } else {
      GCNF_DUMP_N(name(), "unregisterAllCallbacks: ... unregister callbacks for obserserver "<<observer<<" failed (error "<<th->get_mError()<<")!");
      SC_REPORT_WARNING(name(), "unregisterAllCallbacks: The macro GC_UNREGISTER_CALLBACKS in module destructor which unregisters all callbacks has failed!");
    }
  }
  
protected:

  /// Iterates the observer callback map and makes the callbacks.
  /**
   * @param search     Search string for the map (parameter name or dummy for new parameters).
   * @param par_name   Name of the changed parameter.
   * @param value      New value of the changed parameter.
   */
  void makeCallbacks(const std::string &search, const std::string &par_name, const std::string &value) {
    CallbAdapt_b *callb;
    observerCallbackMap::iterator it;
    std::pair<observerCallbackMap::iterator, observerCallbackMap::iterator> begin_end;        
    begin_end = m_observer_callback_map.equal_range(search);
    for (it = begin_end.first;  it != begin_end.second;   ++it) {
      GCNF_DUMP_N(name(), "transport: callback for parameter "<<search.c_str());      
      callb = (*it).second;
      // Make call
      callb->call(par_name, value);
    }
  }

  /// Returns if a private config API is existing for the module owning this API
  /**
   * @return If a private config API is existing for the module owning this API
   */
  bool check_for_private_Api() {
    cnf_api_if* api = getApiInstance(get_parent_sc_module(this));
    if (dynamic_cast<GCnf_private_Api_TMPL*>(api)) {
      return true;
    }
    return false;
  }

  /// Help function which registers this API at the plugin to be informed about new parameters
  /**
   * Automatically checks if this is already done.
   */
  void register_as_new_param_observer() {
    // If we are not yet registered for observing new parameters
    if (!m_registered_as_new_param_observer) {
      // create Transaction an send it to config plugin
      ControlTransactionHandle th = m_gc_port.createTransaction();
      th->set_mService(CONFIG_SERVICE);
      th->set_mCmd(CMD_REGISTER_NEW_PARAM_OBSERVER);
      th->set_mSpecifier(DUMMY_NAME); // e.g. "TestIP1.TestParam1"
      
      m_gc_port->transport(th);
      
      ControlValue val = th->get_mValue();
      if (th->get_mError() == 0) {
        GCNF_DUMP_N(name(), "getNewUpdateEvent: ... success");
      } else {
        SC_REPORT_WARNING(name(), "getNewUpdateEvent: ... registering observer failed!");
        throw RegisterObserverFailedException("Get new update event failed: Registering new param observer failed.", DUMMY_NAME);
      }
      
      m_registered_as_new_param_observer = true;
    }
  }
  
public:

  // ////////////////   deal the initialize-mode   ////////////////////////// ///

  /// Implements initialize_if, deal the initialize-mode.
  void start_initialize_mode() {
    // removed old code
  }

  /// Implements initialize_if, deal the initialize-mode.
  void end_initialize_mode() {
    initialize_mode = false;
  }

  // //////////////// command_if methods ////////////////////////////////////

  /// Returns the name of the API.
  const std::string getName()
  {
    return name();
  }

  /// Returns the name of the specified command.
  const std::string getCommandName(unsigned int cmd)
  {
    return ConfigPlugin_T::getCmdName(cmd);
  }

  /// Return a description of the specified command.
  const std::string getCommandDescription(unsigned int cmd)
  {
    return ConfigPlugin_T::getCmdDesc(cmd);
  }

  // ///////////////////////////////////////////////////////////////////////// //

protected:

  /// Port to send and receive ControlTransactionContiners.
  gc_port m_gc_port;
  
  /// Map to save an observer callback function pointer for each parameter (if needed).
  observerCallbackMap m_observer_callback_map;

  /// If this API is registered to observe new parameters
  bool m_registered_as_new_param_observer;
  
  /// Deprecated event pointer for new parameter event. If NULL, getNewParamEvent not yet called. (But registration may alredy be done if callbacks have been registered) 
  sc_event *m_new_param_event;
  
public:
  
  // if during elaboration
  static bool initialize_mode;
  
};
  template<typename gs_param_base_T, typename GCnf_private_Api_TMPL, template<typename T> class gs_param_T, typename ConfigPlugin_T>
  bool GCnf_Api_t<gs_param_base_T, GCnf_private_Api_TMPL, gs_param_T, ConfigPlugin_T>::initialize_mode = true;
  
  //class ConfigPlugin;
  //typedef GCnf_Api_t<gs_param_base, GCnf_private_Api_T, gs_param, ConfigPlugin> GCnf_Api; // see private API

  
} // end namespace cnf
} // end namespace gs

#endif
