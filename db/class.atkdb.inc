<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage db
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  /**
   * Some defines used for connection statusses, generic error messages, etc.
   */
  define("DB_SUCCESS"           , 0);
  define("DB_UNKNOWNERROR"      , 1);
  define("DB_UNKNOWNHOST"       , 2);
  define("DB_UNKNOWNDATABASE"   , 3);
  define("DB_ACCESSDENIED_USER" , 4);
  define("DB_ACCESSDENIED_DB"   , 5);

  /**
   * Meta flags.
   */
  define('MF_PRIMARY',        1);
  define('MF_UNIQUE',         2);
  define('MF_NOT_NULL',       4);
  define('MF_AUTO_INCREMENT', 8);

  /**
   * Global array containing database instances. Global is necessary because
   * PHP4 doesn't support static class members.
   */
  global $g_dbinstances;


  /**
   * Abstract baseclass for ATK database drivers.
   *
   * Implements some custom functionality and defines some methods that
   * derived classes should override.
   *
   * @author Peter Verhage <peter@achievo.org>
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage db
   *
   */
  class atkDb
  {
    /**
     * The hostname/ip to connect to.
     * @access private
     * @var String
     */
    var $m_host     = "";

    /**
     * The name of the database/schema to use.
     * @access private
     * @var String
     */
    var $m_database = "";

    /**
     * The username for the connection.
     * @access private
     * @var String
     */
    var $m_user     = "";

    /**
     * The password for the connection.
     * @access private
     * @var String
     */
    var $m_password = "";

    /**
     * Contains the current record from the result set.
     * @access private
     * @var array
     */
    var $m_record = array();

    /**
     * Current row number
     * @access private
     * @var int
     */
    var $m_row = 0;

    /**
     * Contains error number, in case an error occurred.
     * @access private
     * @var int
     */
    var $m_errno = 0;

    /**
     * Contains textual error message, in case an error occurred.
     * @access private
     * @var String
     */
    var $m_error = "";

    /**
     * If true, an atkerror is raised when an error occurred.
     *
     * The calling script can use this to stop execution and rollback.
     * If false, the error will be ignored and script execution
     * continues. Use this only for queries that may fail but still
     * be valid.
     * @access private
     * @var boolean
     */
    var $m_haltonerror = true;

    /**
     * Driver name.
     *
     * Derived classes should add their own m_type var to the class
     * definition and put the correct name in it. (e.g. "mysql" etc.)
     * @abstract
     * @access private
     * @var String
     */
    var $m_type = "";

    /**
     * Vendor.
     *
     * This is mainly used to retrieve things like error messages that are
     * common for a vendor (i.e., they do not differ between versions).
     * @abstract
     * @access private
     * @var String
     */
    var $m_vendor = "";

    /**
     * number of affected rows after an update/delete/insert query
     * @access private
     * @var int
     */
    var $m_affected_rows = 0;

    /**
     * array to cache meta-information about tables.
     * @access private
     * @var array
     */
    var $m_tableMeta = array();

    /**
     * The connection is stored in this variable.
     * @access private
     * @var Resource
     */
    var $m_link_id  = 0;

    /**
     * The query statement is stored in this variable.
     * @access private
     * @var Resource
     */
    var $m_query_id = 0;

    /**
     * Auto free result upon next query.
     *
     * When set to true, the previous results are cleared when a new query is
     * executed. It should generally not be necessary to put this to false.
     * @access private
     * @var boolean
     */
    var $m_auto_free = true;

    /**
     * List of error codes that could be caused by an end-user.
     *
     * This type of errors is 'recoverable'. An example is a violation of a
     * unique constraint.
     * @access private
     * @var array
     */
    var $m_user_error = array();

    /**
     * Internal use; error messages from language files are cached here
     * @access private
     */
    var $m_errorLookup = array();

    /**
     * Get the database driver type.
     * @return String driver type
     */
    function getType()
    {
      return $this->m_type;
    }

    /**
     * Get the current connection.
     * @return Connection resource id
     */
    function link_id()
    {
      return $this->m_link_id;
    }

    /**
     * Determine whether an error that occurred is a recoverable (user) error
     * or a system error.
     * @return String "user" or "system"
     */
    function getErrorType()
    {
      if(in_array($this->m_errno,$this->m_user_error))
      {
        return "user";
      }
      return "system";
    }

    /*
     * Get generic atk errorccode
     * @return int One of the ATK DB_* codes.
     */
    function getAtkDbErrno()
    {
      return $this->_translateError();
    }

    /**
     * Get vendor-dependent database error number.
     *
     * Applications should not rely on this method, if they want to be
     * database independent.
     * @return mixed Database dependent error code.
     */
    function getDbErrno()
    {
      return $this->m_errno;
    }

    /**
     * Get vendor-dependent database error message.
     *
     * Applications should not rely on this method, if they want to be
     * database independent.
     * @return String Database dependent error message.
     */
    function getDbError()
    {
      return $this->m_error;
    }

    /**
     * Define custom user error codes.
     *
     * Error codes passed to this method will be treated as recoverable user
     * errors.
     * @param mixed Vendor-dependent database error code
     */
    function setUserError($errno)
    {
      atkdebug("atkdb::setUserError() -> ".$errno);
      $this->m_user_error[]=$errno;
    }

    function errorLookup($errno)
    {
      if (count($this->m_errorLookup)==0)
      {
        $filename = atkconfig("atkroot")."atk/db/languages/".$this->m_vendor."_".atkconfig('language').'.lng';
        @include_once($filename);
        $this->m_errorLookup = $txt_db;
      }
      if (isset($this->m_errorLookup[$errno]))
      {
        return $this->m_errorLookup[$errno];
      }
      return "";
    }

    /**
     * Get localized error message (for display in the application)
     * @return String Error message
     */
    function getErrorMsg()
    {
      $errno = $this->getAtkDbErrno();
      if($errno==DB_UNKNOWNERROR)
      {
        $errstr = $this->errorLookup($this->getDbErrno());
        if($errstr=="")
        {
          $this->m_error = atktext("unknown_error").": ".$this->getDbErrno()." (".$this->getDbError().")";
        }
        else
        {
          $this->m_error=$errstr.($this->getErrorType()=="system"?" (".$this->getDbError().")":"");
        }
        return $this->m_error;
      }
      else
      {
        $tmp_error='';
        switch ($errno)
        {
          case DB_ACCESSDENIED_DB: $tmp_error = sprintf(atktext("db_access_denied_database", "atk"),$this->m_user,$this->m_database);
                                break;
          case DB_ACCESSDENIED_USER: $tmp_error = sprintf(atktext("db_access_denied_user", "atk"),$this->m_user,$this->m_database);
                                     break;
          case DB_UNKNOWNDATABASE: $tmp_error = sprintf(atktext("db_unknown_database", "atk"),$this->m_database);
                                   break;
          case DB_UNKNOWNHOST: $tmp_error = sprintf(atktext("db_unknown_host", "atk"),$this->m_host);
                               break;
        }
        $this->m_error = $tmp_error;
        return $this->m_error;
      }
    }

    /**
     * If haltonerror is set, this will raise an atkerror. If not, it will
     * place the error in atkdebug and continue.
     * @access protected
     */
    function halt()
    {
      if ($this->m_haltonerror)
      {
        if($this->getErrorType()=="system")
        {
          atkdebug("atkdb::halt() on system error");
          if(!in_array($this->m_errno,$this->m_user_error)) $level='critical';
          atkerror($this->getErrorMsg());
          halt($this->getErrorMsg(),$level);
        }
        else
        {
          atkdebug("atkdb::halt() on user error (not halting)");
        }
      }
    }

    /**
     * Get the current query statement resource id.
     * @return Resource Query statement resource id.
     */
    function query_id()
    {
      return $this->m_query_id;
    }

    /**
     * Connect to the database.
     * @return int Connection status
     * @abstract
     */
    function connect()
    {
      return DB_UNKNOWNERROR;
    }


    /**
     * Translate database-vendor dependent error messages into an ATK generic
     * error code.
     *
     * Derived classes should implement this method and translate their error
     * codes.
     * @access private
     * @param mixed Vendor-dependent error code.
     * @return int ATK error code
     */
    function _translateError($errno)
    {
      return DB_UNKNOWNERROR;
    }

    /**
     * Disconnect from database
     * @abstract
     */
    function disconnect()
    {
    }

    /**
     * Commit the current transaction.
     * @abstract
     */
    function commit()
    {
    }

    /**
     * Rollback the current transaction.
     * @abstract
     */
    function rollback()
    {
    }

    /**
     * Parse and execute a query.
     *
     * If the query is a select query, the rows can be retrieved using the
     * next_record() method.
     *
     * @param String $query The SQL query to execute
     * @param int $offset Retrieve the results starting at the specified
     *                    record number. Pass -1 or 0 to start at the first
     *                    record.
     * @param int $limit Indicates how many rows to retrieve. Pass -1 to
     *                   retrieve all rows.
     * @abstract
     */
    function query($query, $offset=-1, $limit=-1)
    {
      return true;
    }

    /**
     * Retrieve the next record in the resultset.
     * @return mixed An array containing the record, or 0 if there are no more
     *               records to retrieve.
     * @abstract
     */
    function next_record()
    {
      return 0;
    }

    /**
     * Lock a table in the database.
     *
     * @param String $table The name of the table to lock.
     * @param String $mode The lock type.
     * @return boolean True if succesful, false if not.
     * @abstract
     */
    function lock($table, $mode="write")
    {
      return 0;
    }

    /**
     * Relieve all locks.
     *
     * @return boolean True if succesful, false if not.
     * @abstract
     */
    function unlock()
    {
      return 0;
    }

    /**
     * Retrieve the number of rows affected by the last query.
     *
     * After calling query() to perform an update statement, this method will
     * return the number of rows that was updated.
     *
     * @return int The number of affected rows
     * @abstract
     */
    function affected_rows()
    {
      return array();
    }

    /**
     * Get the next sequence number of a certain sequence.
     *
     * If the sequence does not exist, it is created automatically.
     *
     * @param $sequence The sequence name
     * @return int The next sequence value
     * @abstract
     */
    function nextid($sequence)
    {
    }

    /**
     * Return the meta data of a certain table HIE GEBLEVEN
     *
     * depending on $full, metadata returns the following values:
     *  -full is false (default):
     *   $result[]:
     *     [0]["table"]  table name
     *     [0]["name"]   field name
     *     [0]["type"]   field type
     *     [0]["len"]    field length
     *     [0]["flags"]  field flags
     *
     *  -full is true:
     *   $result[]:
     *     ["num_fields"] number of metadata records
     *     [0]["table"]  table name
     *     [0]["name"]   field name
     *     [0]["type"]   field type
     *     [0]["len"]    field length
     *     [0]["flags"]  field flags
     *     ["meta"][field name] index of field named "field name"
     *     The last one is used, if you have a field name, but no index.
     *
     * @param $table the table name
     * @param $full all meta data or not
     * @return array with meta data
     */
    function metadata($table, $full=false)
    {

      return array();
    }

    /**
     * Return the available table names
     * @return array with table names etc.
     */
    function table_names()
    {
      return array();
    }

   /**
     * This function checks the database for a table with
     * the provide name
     *
     * @param String $tableName the table to find
     * @return boolean true if found, false if not found
     */
    function tableExists($tableName)
    {
      return false;
    }

    /**
     * Get all rows that are the result
     * of a certain specified query
     *
     * Note: This is not an efficient way to retrieve
     * records, as this will load all records into one
     * array into memory. If you retrieve a lot of records,
     * you might hit the memory_limit and your script will die.
     *
     * @param $query the query
     * @return array with rows
     */
    function getrows($query, $offset=-1, $limit=-1)
    {
      $result = array();

      $this->query($query, $offset, $limit);
      while ($this->next_record())
        $result[] = $this->m_record;

      return $result;
    }

    /**
     * This function indicates what searchmodes the database supports.
     * @return array with search modes
     */
    function getSearchModes()
    {
      // exact match and substring search should be supported by any database.
      // (the LIKE function is ANSI standard SQL, and both substring and wildcard
      // searches can be implemented using LIKE)
      return array("exact","substring", "wildcard","greaterthan","greaterthanequal","lessthan","lessthanequal","between");
    }

    function tableMeta($table)
    {
      if (!isset($this->m_tableMeta[$table]))
      {
        $meta = $this->metadata($table);
        $this->m_tableMeta[$table] = array();
        for ($i = 0, $_i = count($meta); $i < $_i; $i++)
        {
          $meta[$i]['num'] = $i;
          $this->m_tableMeta[$table][$meta[$i]['name']] = &$meta[$i];
        }
      }

      return $this->m_tableMeta[$table];
    }

    /**
      * get NOW() or SYSDATE() equivalent for the current database
      *
      * Every database has it's own implementation to get the current date
      *
      */
    function func_now()
    {
      return "NOW()";
    }

    /**
      * get SUBSTRING() equivalent for the current database.
      */
    function func_substring($fieldname, $startat=0, $length=0)
    {
      return "SUBSTRING($fieldname, $startat".($length!=0?", $length":"").")";
    }

    /**
     * Get TO_CHAR() equivalent for the current database.
     * Each database driver should override this method to perform vendor
     * specific conversion.
     *
     * @param String $fieldname The field to generate the to_char for.
     * @param String $format Format specifier. The format is compatible with
     *                       php's date() function (http://www.php.net/date)
     *                       The default is what's specified by
     *                       $config_date_to_char, or "Y-m-d" if not
     *                       set in the configuration.
     * @return String Piece of sql query that converts a date field to char
     *                for the current database
     */
    function func_datetochar($fieldname, $format="")
    {
      if ($format=="") $format = atkconfig("date_to_char", "Y-m-d");
      return "TO_CHAR($fieldname, ".$this->vendorDateFormat($format).")";
    }

    /**
     * Convert a php date() format specifier to a vendor specific format
     * specifier.
     * The default implementation returns the format as used by many
     * database vendors ('YYYYMMDD HH24:MI'). Databases that use different
     * formatting, should override this method.
     *
     * Note that currently, only the common specifiers Y, m, d, H, h, i and
     * s are supported.
     * @param String $format Format specifier. The format is compatible with
     *                       php's date() function (http://www.php.net/date)
     * @return String Vendor specific format specifier.
     */
    function vendorDateFormat($format)
    {
      $php_fmt = array("Y", "m", "d", "H", "h", "i", "s");
      $db_fmt  = array("YYYY", "MM", "DD", "HH24", "HH12", "MI", "SS");
      return str_replace($php_fmt, $db_fmt, $format);
    }


    /**
     * Get TO_CHAR() equivalent for the current database.
     *
     * TODO/FIXME: add format paramater. Current format is always yyyy-mm-dd hh:mi.
     */
    function func_datetimetochar($fieldname)
    {
      return "TO_CHAR($fieldname, 'YYYY-MM-DD hh:mi')";
    }

    /**
      * Returns the maximum length an identifier (tablename, columnname, etc) may have
      *
      */
    function maxIdentifierLength()
    {
      return 64;
    }

    /**
     * escapes quotes for use in SQL: ' -> '' (and sometimes % -> %%)
     */
    function escapeSQL($string, $wildcard=false)
    {
      $result = str_replace("'","''",$string);
      $result = str_replace("\\","\\\\",$result);
      if ($wildcard == true) $result = str_replace("%","%%",$result);
      return $result;
    }

    /**
     * Create an atkQuery object for constructing queries.
     * @return atkQuery Query class.
     */
    function &createQuery()
    {
      $query = &atknew("atk.db.atk".$this->m_type."query");
      $query->m_db = &$this;
      return $query;
    }


    /**
     * Create an atkDDL object for constructing ddl queries.
     * @return atkDDL DDL object
     */
    function &createDDL()
    {
      atkimport("atk.db.atkddl");
      $ddl = &atkDDL::create($this->m_type);
      $ddl->m_db = &$this;
      return $ddl;
    }

    /**
     * Get database instance.
     *
     * This method instantiates and returns the correct (vendor specific)
     * database instance, depending on the configuration.
     *
     * @static
     * @param String $conn The name of the connection as defined in the
     *                     config.inc.php file (defaults to 'default')
     * @return atkDb Instance of the database class.
     */
    function &getInstance($conn="default", $reset=false)
    {
      global $g_dbinstances;
      if (!isset($g_dbinstances[$conn]) || $reset)
      {
        atkdebug("Creating new db instance for connection $conn");
        $dbconfig = atkconfig("db");
         // include DB stuff
        $g_dbinstances[$conn] = &atknew("atk.db.atk".$dbconfig[$conn]["driver"]."db");
        if (isset($dbconfig[$conn]["db"]))
        {
          $g_dbinstances[$conn]->m_database = $dbconfig[$conn]["db"];
          $g_dbinstances[$conn]->m_user     = $dbconfig[$conn]["user"];
          $g_dbinstances[$conn]->m_password = $dbconfig[$conn]["password"];
          $g_dbinstances[$conn]->m_host     = $dbconfig[$conn]["host"];
          if (isset($dbconfig[$conn]["port"]))
            $g_dbinstances[$conn]->m_port     = $dbconfig[$conn]["port"];
        }
      }
      return $g_dbinstances[$conn];
    }

    /**
     * Replace the current instance of a named connection at runtime with a
     * different connection. This is useful for example for replacing a
     * named database instance with a mock object for testing purposes.
     *
     * @param unknown_type $name
     * @param unknown_type $db
     */
    function &setInstance($name, &$db)
    {
      global $g_dbinstances;
      $olddb = &$g_dbinstances[$name];
      $g_dbinstances[$name] = &$db;
      return $olddb;
    }

  }

?>