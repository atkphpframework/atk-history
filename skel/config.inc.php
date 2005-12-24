<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * This file is the skeleton config file, which you can copy
   * to your application dir and modify if necessary. It contains
   * many default settings that you may adjust to the needs of
   * your application. If you don't want endusers to modify a
   * setting, it is good practice to move them to a separate
   * atkconf.inc file, which you include near the bottom of this
   * configfile.
   * For every configuration variable that ATK uses you can check out
   * atk/defaultconfig.inc.php
   *
   * @package atk
   * @subpackage skel
   *
   * @author Ivo Jansch <ivo@achievo.org>
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */  
   
  /**
   * The unique application identifier (used for sessions)
   * @var String
   */
  $config_identifier = "atkapp";

  //----------------- DATABASE CONFIGURATION --------------------

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
  $config_db["default"]["driver"]   = "mysql";
  
  /**
   * The IP or hostname of the database host
   * @var String
   */
  $config_db["default"]["host"]     = "localhost";
  
  /**
   * The name of the database to use
   * @var String
   */
  $config_db["default"]["db"]       = "";
  
  /**
   * The name of the database user with which to connect to the host
   * @var String
   */
  $config_db["default"]["user"]     = "";
  
  /**
   * The password for the database user, used to connect to the databasehost
   * @var String
   */
  $config_db["default"]["password"] = "";

  /**
   * In admin pages, atk shows you a number of records with previous and
   * next buttons. You can specify the number of records to show on a page.
   * @var int
   */
  $config_recordsperpage=25;

  //----------------- DEBUGGER CONFIGURATION ----------------------

  /**
   * The automatic error reporter
   * Error reports are sent to the given email address.
   * If you set this to "", error reporting will be turned off.
   * WARNING: DO NOT LEAVE THIS TO THE DEFAULT ADDRESS OR PREPARE TO BE
   * SEVERELY FLAMED!
   * @var String
   */
  // $config_mailreport = "ivo@ibuildings.nl";

  /**
   * The debug level.
   * 0 - No debug information
   * 1 - Print some debug information at the bottom of each screen
   * 2 - Print debug information, and pause before redirects
   * @var int
   */
  $config_debug = 1;

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

  //----------------- LAYOUT CONFIGURATION --------------------

  /**
   * The theme defines the layout of your application. You can see which
   * themes there are in the directory atk/themes.
   * @var String
   */
  $config_defaulttheme = "outlook";

  /**
   * The language of the application. You can use any language for which
   * a language file is present in the atk/languages directory.
   * @var String
   */
  $config_language="en";

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
   * Lists that are obligatory, by default have no 'Select none' option. In
   * some applications, this leads to the user just selecting the first item
   * since that is the default. If this is a problem set this config variable
   * to true; this will add a 'Select none' option to obligatory lists so the
   * user is forced to make a selection.
   * @var bool
   */
  $config_list_obligatory_null_item = false;

  //----------------- SECURITY CONFIGURATION --------------------

  /**
   * The type of authentication (user/password verification) to use.
   * Currently supported are:
   * "none"   - no authentication
   * "config" - users / passwords are set in this configfile
   * "db"     - users / passwords are stored in a table in the database.
   * "imap"   - users / passwords are stored in the IMAP server
   * "pop3"   - users / passwords are stored in the POP3 server
   * "ldap"   - users / passwords are stored in an LDAP server
   * "server" - authentication is done by the webserver (.htaccess)
   * if you need to use multiple authentication types list them delimited by comma
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
   * @var bool
   */
  $config_authentication_md5 = true;

  /**
   * This parameter specified whether passwords are stored using the
   * php/perl crypt() function. Applications using this are Bugzilla.
   * Setting this to true, and setting $config_authentication_md5 to
   * false, allows users to login to applications where the database
   * uses crypted passwords.
   * @var bool
   */
  $config_auth_usecryptedpassword = false;

  /**
   * If you specify an administrator password here, you are always able
   * to login using user 'administrator' and the specified password,
   * regardless of the type of authentication used!
   * if you set it to nothing (""), administrator login is disabled,
   * and only valid users are allowed to login (depending on the type of
   * authentication used).
   * @var String
   */
  $config_administratorpassword = "demo";

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
   */
  $config_restrictive = true;

  /**
   * If set to true, a cookie with username/password is written, so
   * users will stay logged in, even if they close their browser.
   * @var bool
   */
  $config_authentication_cookie = false;

  // If you use "db" as authentication type, you can set the table and fields
  // from which atk should read the username and password information.
  
  /**
   * The table to use for user authentication
   * @var String
   */
  $config_auth_usertable   = "user";
  
  /**
   * The field where the login name of the user is stored
   * @var String
   */
  $config_auth_userfield   = "userid";
  
  /**
   * The field where the password of the user is stored
   * @var String
   */
  $config_auth_passwordfield = "password";

  // If you work with groups/levels  you need these parameters
  
  /**
   * Table where levels are stored. Defaults to usertable
   * @var String
   */
  $config_auth_leveltable  = "users";
  
  /**
   * The field where we store the level of a user (?)
   * @var String
   */
  $config_auth_levelfield  = "entity";

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
   */
  // $config_auth_mail_virtual = false;

  /**
   * Mail suffix, if mail_virtual is set to true.
   */
  //$config_auth_mail_suffix = "ibuildings.nl";

  /**
   * Mail server name
   */
  // $config_auth_mail_server = "localhost";

  /*
   * Port of the mail server (default is 110 (pop3) but you can set it
   * to 143 (imap) or something else.
   */
  // $config_auth_mail_port = 143;

  // if you use "ldap" as authentication, these parameters are nessesary

  /**
   * The hostname of the LDAP server
   * @var String
   */
  // $config_auth_ldap_host = "";
  
  /**
   * The context of the LDAP server (?)
   * @var String
   */
  // $config_auth_ldap_context = "";

  /**
   * The application root
   * if you're using urlrewrites within your httpd or htaccess configuration i think this should be '/'
   * be careful with this setting because it could create a major securityhole.
   * It is used to set the cookiepath when using PHP sessions.
   * @var String
   */
  $config_application_root = "/";

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

  /**
   * If you have config_authentication set to "config", you may now specify
   * a set of usernames and passwords (in plain text).
   * Example:
   * atkConfig::addUser("harry","password");

   * If securityscheme is "level" or "group", you may also specify the
   * level or group as third parameter:
   * atkConfig::addUser("harry","password",3);
   * atkConfig::addUser("harry","password","admins");
   */

 // -------------- DOCUMENT WRITER CONFIGURATION ---------------

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

  // --------- DATE INTERNATIONALISATION CONFIGURATION ---------

  /**
   * @todo document me!
   */
  $config_supported_languages = array("EN","NL","DE");
  
  /**
   * @todo document me!
   */
  $config_defaultlanguage="EN";

?>
