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
   * @todo Make sure EVERY config variable is located here and properly
   * documented.
   *
   * @version $Revision$
   * $Id$
   */

  /********************* FILE LOCATIONS & PATHS ******************************/

  /**
   * The application root
   * if you're using urlrewrites within your httpd or htaccess configuration i think this should be '/'
   * be careful with this setting because it could create a major securityhole.
   * It is used to set the cookiepath when using PHP sessions.
   * @var String
   */
  $config_application_root = "/";

  if ($config_atkroot == "")
  {
    /**
     * The root of the ATK application, where the atk/ directory resides
     * @var String The root
     */
     $config_atkroot = "./";
  }

  /**
   * Module path (without trailing slash!) where the modules directory resides,
   * defaults to the atk root, so not IN the atk/ directory, but more likely in
   * the application root
   * @var String
   */
  $config_module_path = $config_atkroot."modules";

  /**
   * @todo add documentation!
   */
  $config_corporate_node_base = "";
  
  /**
   * @todo add documentation!
   */
  $config_corporate_module_base = "";

  /**
   * The location of a directory that is writable to ATK and that ATK can
   * store it's temporary files in.
   * @var String
   */
  $config_atktempdir = $config_atkroot."atktmp/";

  /**
   * The location of the module specific configuration files.
   * @var String
   */
  $config_configdir = $config_atkroot."configs/";

  /**
   * Use the given meta policy as the default meta policy
   * @var String
   */
  $config_meta_policy = "atk.meta.atkmetapolicy";

  /**
   * Use the given meta handler as the default meta handler
   * @var String
   */
  $config_meta_handler = "atk.meta.atkmetahandler";

  /**
   * Use the given meta grammar as the default meta grammar
   * @var String
   */
  $config_meta_grammar = "atk.meta.grammar.atkmetagrammar";

  /************************** DATABASE SETTINGS ******************************/

  /**
   * The IP or hostname of the database host
   * @var String
   */
  $config_db["default"]["host"] = "localhost";

  /**
   * The name of the database to use
   * @var String
   */
  $config_db["default"]["db"] = "";

  /**
   * The name of the database user with which to connect to the host
   * @var String
   */
  $config_db["default"]["user"] = "";

  /**
   * The password for the database user, used to connect to the databasehost
   * @var String
   */
  $config_db["default"]["password"] = "";

  /**
   * Wether or not to use a persistent connection.
   *
   * Note that this is usefull if you don't have a lot of applications doing
   * this as the application won't constantly have to connect to the database.
   * However, the database server won't be able to handle a lot of persistent
   * connections.
   * @var boolean
   */
  $config_databasepersistent = true;

  /**
   * The databasetype.
   *
   * Currently supported are:
   *   mysql:   MySQL (3.X ?)
   *   mysql41: MySQL > 4.1.3
   *   oci8:    Oracle 8i
   *   oci9:    Oracle 9i and 10g
   *   pgsql:   PostgreSQL
   *   mssql:   Microsoft SQL Server
   * @var String
   */
  $config_db["default"]["driver"]="mysql";

  /**
   * Backwardscompatibility setting. Set this to MYSQL_BOTH if your
   * software relies on numerical indexes (WHICH IS A BAD IDEA!!)
   * @var int
   */
  $config_mysqlfetchmode = defined("MYSQL_ASSOC") ? MYSQL_ASSOC : 0;

  /**
   * Backwardscompatibility setting. Set this to PGSQL_BOTH if your
   * software relies on numerical indexes (WHICH IS A BAD IDEA!!)
   * @var int
   */
  $config_pgsqlfetchmode = defined("PGSQL_ASSOC") ? PGSQL_ASSOC : 0;

  /********************************** SECURITY *******************************/

  /**
   * If you specify an administrator password here, you are always able
   * to login using user 'administrator' and the specified password,
   * regardless of the type of authentication used!
   * if you set it to nothing (""), administrator login is disabled,
   * and only valid users are allowed to login (depending on the type of
   * authentication used).
   * @var String
   */
  $config_administratorpassword = "";

  /**
   * The password to use for guest login.
   * A guest password that is empty will *DISABLE* guest login!
   * @var String
   */
  $config_guestpassword = "";

  /**
   *
   * @var String
   */
  $config_authentication = "none";

  /**
   * The type of authorization (what is a user allowed to do?)
   * Normally this will be the same as the authentication, but in
   * special cases like POP3 authentication you might want to
   * authorize via a table in the database.
   * $config_authorization = "none";

   * NOTE, the following options are only useful if authentication is not
   * set to "none".

   * This parameter specifies whether the passwords are stored as an md5
   * string in the database / configfile / whatever.
   * If set to false, the passwords are assumed to be plain text.
   * Note: Not all authentication methods support md5! e.g, if you use
   *       pop3 authentication, set this to false.
   * Note2: If set to false, and authentication_cookie is set to true,
   *        the password in the cookie will be stored plaintext!!!
   *
   * @var boolean
   */
  $config_authentication_md5 = true;
  
  /**
   * This parameter specified whether passwords are stored using the
   * php/perl crypt() function. Applications using this are Bugzilla.
   * Setting this to true, and setting $config_authentication_md5 to
   * false, allows users to login to applications where the database
   * uses crypted passwords.
   * @var boolean
   */
  $config_auth_usecryptedpassword = false;

  /**
   * If set to true, a cookie with username/password is written, so
   * users will stay logged in, even if they close their browser.
   *
   * @var boolean
   */
  $config_authentication_cookie = false;

  /**
   * The default cookie expiry time (in minutes) (7 days)
   * @var int
   */
  $config_authentication_cookie_expire = 10080;

  /**
   *
   * @var boolean
   */
  $config_authentication_session = true;

  /**
   * The security scheme is used to determine who is allowed to do what.
   * Currently supported:
   * "none"   - anyone who is logged in may do anything.
   * "level"  - users have a certain level, and certain features of the
   *            application require a minimum level.
   * "group"  - users belong to a group, and certain features may only
   *            be executed by a specific group.
   * @var String
   */
  $config_securityscheme = "none";

  /**
   * If config_restrictive is set to true, access is denied for all features
   * for which no access requirements are set. If set to false, access is
   * always granted if no access requirements are set.
   *
   * @var boolean
   */
  $config_restrictive = true;

  /**
   *
   * @var boolean
   */
  $config_security_attributes = false;

  /**
   * By default, there is no 'grantall' privilege. Apps can set this if necessary.
   * Syntax: "module.nodename.privilege"
   */
  $config_auth_grantall_privilege = "";

  /**
   * Atk can write security events to a logfile.
   * There are several values you can choose for $config_logging.
   * 0 - No logging
   * 1 - Log logins
   * 2 - Log actions ("User x performed action x on module y")
   * @var int
   */
  $config_logging = 0;

  /**
   * The file to write the log to
   * @var String
   */
  $config_logfile = "/tmp/atk-security.log";

  /************************** AUTHENTICATION *********************************/
  
  /**
   * The table to use for user authentication
   * @var String
   */
  $config_auth_usertable   = "user";

  /**
   * Table where levels are stored. Defaults to usertable
   * @var String
   */
  $config_auth_leveltable  = "";

  /**
   *
   * @var String
   */
  $config_auth_accesstable = "access";

  /**
   * The field where the login name of the user is stored
   * @var String
   */
  $config_auth_userfield   = "userid";

  /**
   * Primary key of usertable
   * @var String
   */
  $config_auth_userpk = "userid";

  /**
   * The field where the password of the user is stored
   * @var String
   */
  $config_auth_passwordfield = "password";

  /**
   *
   * @var String
   */
  $config_auth_languagefield   = "lng";

  /**
   *
   * @var String
   */
  $config_auth_accountdisablefield = "";

  /**
   * The field where we store the level of a user (?)
   * @var String
   */
  $config_auth_levelfield = "entity";

  /**
   * Name of table containing the groups.
   * (only necessary to support hierarchical groups!).
   * @var String
   */
  $config_auth_grouptable = "";

  /**
   * Name of primary key attribute in group table.
   * (only necessary to support hierarchical groups!)
   * @var String
   */
  $config_auth_groupfield = "";

  /**
   * Name of parent attribute in group table.
   * (only necessary to support hierarchical groups!)
   * @var String
   */
  $config_auth_groupparentfield = "";

  /**
   * Default pop3 port
   * @var String
   */
  $config_auth_mail_port = "110";

  /**
   * No vmail.
   * @var boolean
   */
  $config_auth_mail_virtual = false;

  /**
   * Use bugzilla-style crypted password storage
   * @var boolean
   */
  $config_auth_usecryptedpassword = false;

  /**
   * When changerealm is true, the authentication realm is changed on every
   * login.
   *
   * Advantage: the user is able to logout using the logout link.
   * Disadvantage: browser's 'remember password' feature won't work.
   *
   * This setting only affects the http login box, so it is only relevant if
   * $config_auth_loginform is set to false.
   *
   * The default is true for backwardscompatibility reasons. For new
   * applications, it defaults to false since the skel setting is set to false
   * by default.
   * @var boolean
   */
  $config_auth_changerealm = true;

  /**
   * The maximum amount a visitor may try to login.
   * 0 = no maximum.
   * @var int
   */
  $config_max_loginattempts = 5;


  /**
   *
   * @var boolean
   */
  $config_auth_dropdown = false;

  /**
   *
   * @var String
   */
  $config_auth_userdescriptor = "[".$config_auth_userfield."]";

  /**
   * This parameter can be used to specify a where clause which will be used
   * to validate users login credentials
   * @var String
   */
  $config_auth_accountenableexpression = "";
  
  /**
   * Setting this to true will make ATK use a loginform instead of a browser
   * popup.
   * @var bool
   */
  $config_auth_loginform = true;
  
  /**
   * The maximum amount a visitor may try to login.
   * 0 = no maximum.
   * @var int
   */
  $config_max_loginattempts = 5;

  /**
   * if you use "pop3" or "imap" as authentication, you have to fill in
   * these parameters:

   * Set this to true if you have virtual mail domains.
   * Atk will append '@' and the config_auth_mail_suffix
   * to the login name.
   * @var bool
   */
   $config_auth_mail_virtual = false;

  /**
   * Mail suffix, if mail_virtual is set to true.
   * @var String
   */
  $config_auth_mail_suffix = "";

  /**
   * Mail server name
   * @var String
   */
   $config_auth_mail_server = "localhost";

  /**
   * Port of the mail server (default is 110 (pop3) but you can set it
   * to 143 (imap) or something else.
   * @var int
   */
   $config_auth_mail_port = 143;

  /**
   * The hostname of the LDAP server
   * @var String
   */
   $config_auth_ldap_host = "";
   
   /**
    * The context of the LDAP server (?)
    * @var String
    */
   $config_auth_ldap_context = "";


  /***************************** LDAP settings *******************************/
  /**
   * To use LDAP you should fill this config_variables with the right values
   */

  /**
   *
   * @var String
   */
  $config_authentication_ldap_host    = "";

  /**
   *
   * @var String
   */
  $config_authentication_ldap_context = "";

  /**
   *
   * @var String
   */
  $config_authentication_ldap_field   = "";

  /***************** DEBUGGING AND ERROR HANDLING ****************************/

  /**
   * The debug level.
   * 0 - No debug information
   * 1 - Print some debug information at the bottom of each screen
   * 2 - Print debug information, and pause before redirects
   * @var int
   */
  $config_debug = 0;

  /**
   *
   * @var String
   */
  $config_debuglog = "";

  /**
   * Smart debug parameters. Is used to dynamically enable debugging for
   * certain IP addresses or if for example the special atkdebug[key] request
   * variable equals a configured key etc. If smart debugging is enabled
   * you can also change the debug level dynamically using the special
   * atkdebug[level] request variable.
   *
   * @example $config_smart_debug[] = array("type" => "request", "key" => "test");
   *          $config_smart_debug[] = array("type" => "ip", "list" => array("10.0.0.4"));
   */
  $config_smart_debug = array();

  /**
   *
   * @var boolean
   */
  $config_display_errors = true;

  /**
   *
   * @var String
   */
  $config_halt_on_error = "critical";

  /**
   * The automatic error reporter
   * Error reports are sent to the given email address.
   * If you set this to "", error reporting will be turned off.
   * WARNING: DO NOT LEAVE THIS TO THE DEFAULT ADDRESS OR PREPARE TO BE
   * SEVERELY FLAMED!
   * Automatic error reporting is turned off by default.
   * @var String
   */
  $config_mailreport = "";

  /************************************ LAYOUT *******************************/

  /**
   * The theme defines the layout of your application. You can see which
   * themes there are in the directory atk/themes.
   * @var String
   */
  $config_defaulttheme = "default";
  
  /**
   * The layout of the menu to use.
   * This has to be supported by the theme you are using otherwise
   * the default menu layout will be used.
   * @var String
   */
  $config_menu_layout = "plain";

  /**
   * If the users are using IE, then the application can be run in fullscreen
   * mode. Set the next variable to true to enable this:
   * @var bool
   */
  $config_fullscreen = false;

  /**
   * Whatever tabs are enabled or not
   * @var boolean
   */
  $config_tabs = true;

  /**
   * In admin pages, atk shows you a number of records with previous and
   * next buttons. You can specify the number of records to show on a page.
   * @var int
   */
  $config_recordsperpage=25;

  /**
   * Display a 'stack' of the user activities in the top right corner.
   * @var boolean
   */
  $config_stacktrace = true;

  /**
   * The maximum length of an HTML input field generated by atkAttribute or
   * descendants
   * @var int
   */
  $config_max_input_size = 70;

  /*********************************** OUTPUT ********************************/

  /**
   * Set to true, to output pages gzip compressed to the browser if the
   * browser supports it.
   */
  $config_output_gzip = false;

  /********************************** LANGUAGE *******************************/

  /**
   * The language of the application. You can use any language for which
   * a language file is present in the atk/languages directory.
   * @var String
   */
  $config_language="en";

  /**
   * The default language for the application
   * @var String
   */
  $config_defaultlanguage = "en";

  /**
   * The name of the directory atkLanguage can find it's languages in.
   * @var String
   */
  $config_language_basedir = "languages/";

  /**
   * True: one language switch attributes automatically switches all others on
   * screen.
   * False: each language switch attributes operates only on it's own node
   * @var boolean
   */
  $config_multilanguage_linked = true;

  /**
   * Module/node checking for strings in atkLanguage (if you don't know, don't
   * change)
   * comment out to disable checking for module of node
   * 1 to check just for node
   * 2 to check for module and node
   * @var String
   */
  $config_atklangcheckmodule = 2;

  /**
   * Where ATK should look for it's supported languages
   *
   * In your own application you should probably make this the module
   * with the most language translations.
   * Leaving this empty will turn off functionality where we check
   * for the user language in the browser or in the user session and will
   * make sure the application is always presented in the default language.
   * This config var also accepts 2 'special' modules:
   * - atk (making it use the languages of ATK)
   * - langoverrides (making it use the language overrides directory)
   *
   * @var String
   */
   //$config_supported_languages_module = $config_atkroot.'atk/languages/';
   $config_supported_languages_module = '';

  /********************* TEMPLATE ENGINE CONFIGURATION ***********************/

  /**
   * By default all templates are described by their relative
   * path, relative to the applications' root dir.
   * @var String
   */
  $config_tplroot = $config_atkroot;

  /**
   *
   * @var boolean
   */
  $config_tplcaching = false;

  /**
   * default one hour
   * @var int
   */
  $config_tplcachelifetime = 3600;
  /**
   *
   * @var String
   */
  $config_tplcompiledir = $config_atktempdir."compiled/tpl/";

  /**
   *
   * @var String
   */
  $config_tplcachedir = $config_atktempdir."tplcache/";

  /**
   * Check templates to see if they changed
   * @var String
   */
  $config_tplcompilecheck = "true";

  /**
  * Use subdirectories for compiled and cached templates
  */
  $config_tplusesubdirs = false;

  /****************** MISCELLANEOUS CONFIGURATION OPTIONS ********************/

  /**
   * The unique application identifier (used for sessions)
   * @var String
   * @todo update this bit of documentation as it doesn't really say much
   */
  $config_identifier = "default";

  /**
   * Lock type, only supported type at this time is "db".
   * If empty locking is disabled.
   * @var String
   * @todo update this bit of documentation as it doesn't really say much
   */
  $config_lock_type = "";

  /**
   * The default encryption method for atkEncryption
   * @var String
   * @todo update this bit of documentation as it doesn't really say much
   */
  $config_encryption_defaultmethod = "base64";

  /**
   * The default searchmode
   * @var String
   * @todo update this bit of documentation as it doesn't really say much
   */
  $config_search_defaultmode = "substring";

  /**
   * Wether or not to enable Internet Explorer extensions
   * @var boolean
   * @todo update this bit of documentation as it doesn't really say much
   */
  $config_enable_ie_extensions = false;

  /**
   * Files that are allowed to be included by the include wrapper script
   * NOTE: this has nothing to do with useattrib and userelation etc.!
   * @var Array
   */
  $config_allowed_includes = array("atk/lock/lock.php", "atk/lock/lock.js.php", "atk/javascript/class.atkdateattribute.js.inc",
                                               "atk/popups/help.inc", "atk/popups/colorpicker.inc");

  /**
   * Forces the themecompiler to recompile the theme all the time
   * This can be handy when working on themes.
   * @var boolean
   */
  $config_force_theme_recompile = false;

  /**
   * Wether or not to use the keyboardhandler for attributes and the recordlist
   * When set to true, arrow keys can be used to navigate through fields and
   * records, as well as shortcuts 'e' for edit, 'd' for delete, and left/right
   * cursor for paging. Note however, that using cursor keys to navigate
   * through fields is not standard web application behaviour.
   * @var int
   */
  $config_use_keyboard_handler = false;

  /**
   * Session cache expire (minutes)
   * @var int
   */
  $config_session_cache_expire = 180;

  /**
   * Session cache limiter
   *
   * Possible values:
   * - nocache
   * - public  (permits caching by proxies and clients
   * - private (permits caching by clients
   * - private_no_expire (permits caching by clients but not sending expire
   *   headers >PHP4.2.0)
   * @var String
   */
  $config_session_cache_limiter = "nocache";

  /**
   * Default sequence prefix.
   * @var String
   */
  $config_database_sequenceprefix = "seq_";

  /**
   * Make the recordlist use a javascript
   * confirm box for deleting instead of a seperate page
   * @var boolean
   */
  $config_javascript_confirmation = false;

  /**
   * This should be turned on when an application makes use
   * of OpenSSL encryption (atk.security.encryption.atkopensslencryption)
   * It makes sure that the user password is available in the session
   * for the private key.
   * @var boolean
   */
  $config_enable_ssl_encryption = false;
  
  /**
   * Lists that are obligatory, by default have no 'Select none' option. In
   * some applications, this leads to the user just selecting the first item
   * since that is the default. If this is a problem set this config variable
   * to true; this will add a 'Select none' option to obligatory lists so the
   * user is forced to make a selection.
   * @var bool
   */
  $config_list_obligatory_null_item = false;

  /**
   * For document attributes, ATK automatically searches for template
   * documents in a specific directory. The base directory to search in
   * can be specified below. The document templates must be put in a
   * specific directory structure under this base directory: first of all
   * a subdirectory must be made for every module for which you want to
   * include document templates (equal to the modulename of that module, as
   * set in config.inc.php). Then a subdirectory in that directory must be
   * made according to the name of the node for which you want to include
   * document templates. In this subdirectory you can put your document
   * template files. So if you have $config_doctemplatedir set to
   * "doctemplates/", then you can put your documents in
   * "doctemplates/modulename/nodename/".
   */
  $config_doctemplatedir = "doctemplates/";

  /**
   * @todo document me!
   */
  $config_supported_languages = array("EN","NL","DE");
  
  /**
   * @todo document me!
   */
  $config_defaultlanguage="EN";

  /**
   * Enable / disable sending of e-mails (works only if the atk.utils.atkMail::mail
   * function has been used for sending e-mails).
   * @var boolean
   */
  $config_mail_enabled = true;
  
  /**
   * Default extended search action. This action can always be overriden
   * in the node by using $node->setExtendedSearchAction. At this time
   * (by default) the following values are supported: 'search' or 'smartsearch'
   * 
   * @var string
   */
  $config_extended_search_action = 'search';
?>
