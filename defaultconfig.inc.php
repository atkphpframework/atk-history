<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * The file contains the default values for most configuration 
   * settings.
   * 
   * @package atk
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  // the application identifier (used for sessions)
  $config_identifier   = "default";

  // The application root
  $config_application_root = "/";
  if ($config_atkroot == "") $config_atkroot = "./";

  // set several default configuration options
  $config_databasehost = "localhost";
  $config_databasename = "";
  $config_databaseuser = "";
  $config_databasepassword = "";
  $config_databasepersistent = true;

  // mysql, oci8 or pgsql
  $config_database="mysql";

  // default sequence prefix. 
  $config_database_sequenceprefix = "seq_";

  $config_language="en";
  $config_recordsperpage=25;

  // lock type
  $config_lock_type = "dummy";

  // security
  $config_authentication = "none";
  $config_authentication_md5 = true;
  $config_authentication_cookie = false;
  $config_authentication_cookie_expire = 10080; // the default cookie expiry time (in minutes) (7 days)
  $config_authentication_session = true;
  $config_securityscheme = "none";
  $config_restrictive = true;
  $config_security_attributes = false;

  $config_auth_usertable   = "user";
  $config_auth_leveltable  = ""; // defaults to usertable
  $config_auth_accesstable = "access";
  $config_auth_userfield   = "userid";
  $config_auth_userpk = "userid";//primary key of usertable
  $config_auth_passwordfield = "password";
  $config_auth_accountdisablefield = "";
  $config_auth_levelfield = "entity";
  $config_auth_mail_port = "110"; // default pop3 port
  $config_auth_mail_virtual = false; // no vmail.
  $config_auth_usecryptedpassword = false; // use bugzilla-style crypted password storage
  $config_max_loginattempts = 5; // 0 = no maximum.
  $config_auth_dropdown = false;
  $config_auth_userdescriptor = "[".$config_auth_userfield."]";
  // this parameter can be used to specify a where clause which will be used to validate users login credentials
  $config_auth_accountenableexpression = "";

  // LDAP settings
  // to use LDAP you should fill this config_variabels with the right values
  $config_authentication_ldap_host    = "";
  $config_authentication_ldap_context = "";
  $config_authentication_ldap_field   = "";

  // Encrytion method.
  $config_encryption_defaultmethod = "base64";

  $config_logging = 0; // no logging;
  $config_logfile = "/tmp/atk-security.log";

  $config_debug = 0;
  $config_debuglog = "/home/boy/fundingtest.log";
  $config_smart_debug = array();
  $config_display_errors = true;
  $config_halt_on_error = "critical";

  // Layout config
  $config_doctype = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
        "http://www.w3.org/TR/html4/loose.dtd">';
  $config_menu_delimiter = "<br>";
  $config_menu_pos = "left";
  $config_menu_layout = "default";
  $config_menu_align = "center";
  $config_top_frame = 0; // 0 = no   - 1 = yes
  $config_defaulttheme = "default";

  // language config
  $config_defaultlanguage = "en";
  $config_language_basedir = "languages/";
  $config_language_debugging = false; 
  
  $config_multilanguage_linked = true; // True: one language switch attributes automatically switches all others on screen.
                                       // False: each language switch attributes operates only on it's own node

  // Display a 'stack' of the user activities in the top right corner.
  $config_stacktrace = true;

  // An administrator password that is empty will *DISABLE* administrator login!
  $config_administratorpassword = "";

  // A guest password that is empty will *DISABLE* guest login!
  $config_guestpassword = "";

  // Module path (without trailing slash!)
  $config_module_path = $config_atkroot."modules";

  // Automatic error reporting is turned off by default.
  $config_mailreport = "";

  $config_search_defaultmode = "substring";

  // Whether the action links in a recordlist appear left or right
  $config_recordlist_orientation  = "left";
  $config_recordlist_vorientation = "middle";
	
  // Use icons for action links or not  
  $config_recordlist_icons = "true";
	
  // Make the recordlist use a javascript
  // confirm box for deleting instead of a seperate page
  $config_javascript_confirmation = false;
	
  $config_enable_ie_extensions = false;

  // Whatever tabs are enabled or not
  $config_tabs = true;  

  // Backwardscompatibility setting. Set this to MYSQL_BOTH if your
  // software relies on numerical indexes (WHICH IS A BAD IDEA!!)
  $config_mysqlfetchmode = MYSQL_ASSOC;
  
  $config_atktempdir = $config_atkroot."atktmp/";
  
  // Template engine configuration
  $config_tplroot = $config_atkroot; // By default all templates are described by their relative
                                     // path, relative to the applications' root dir.
  $config_tplcaching = false;
  $config_tplcachelifetime = 3600; // default one hour
  $config_tplcompiledir = $config_atktempdir."compiled/tpl/";
  $config_tplcachedir = $config_atktempdir."tplcache/";
  $config_tplcompilecheck = "true"; // check templates to see if they changed

  // files that are allowed to be included by the include wrapper script
  // NOTE: this has nothing to do with useattrib and userelation etc.!
  $config_allowed_includes = array("atk/lock/lock.php", "atk/lock/lock.js.php", "atk/javascript/class.atkdateattribute.js.inc",
                                               "atk/popups/help.inc", "atk/popups/colorpicker.inc");

  // fullscreen mode (IE only)
  $config_fullscreen = false;
  
  // Module/node checking for strings in atkLanguage (if you don't know, don't change)
  // comment out to disable checking for module of node
  // 1 to check just for node
  // 2 to check for module and node
  $config_atklangcheckmodule = 2;

  // To force the themecompiler to recompile the theme all the time
  // uncomment the config variable below.
  // This can be handy when working on themes.
  // $config_force_theme_recompile = true;
  
  // Wether or not to use the keyboardhandler for attributes and the recordlist
  // Defaults to 1 (true), comment or set to null to remove keyboard handler
  $config_use_keyboard_handler = 1;
  
  // Use the given meta policy as the default meta policy
  $config_meta_policy = "atk.meta.atkmetapolicy";
  
  // Use the given meta handler as the default meta handler
  $config_meta_handler = "atk.meta.atkmetahandler";
  
  // Session cache expire (minutes)
  $config_session_cache_expire = 180;
  
  // Session cache limiter
  // possible values:
  // - nocache
  // - public  (permits caching by proxies and clients
  // - private (permits caching by clients
  // - private_no_expire (permits caching by clients but not sending expire headers >PHP4.2.0)
  $config_session_cache_limiter = "nocache";
  
?>
