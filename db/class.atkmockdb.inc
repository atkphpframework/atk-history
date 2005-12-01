<?php

  class atkMockDb extends atkDb 
  {
    var $m_type = "mock";
    var $m_results = array();
    var $m_regex_results = array();
    var $m_query_history = array();
    
    function atkMockDb()
    {
      return true;
    }
    
    /**
     * Get the current connection.
     * @return Connection resource id
     */
    function link_id()
    {
      return 0;
    }

    /**
     * Determine whether an error that occurred is a recoverable (user) error
     * or a system error.
     * @return String "user" or "system"
     */
    function getErrorType()
    {
      return "system";
    }

    /*
     * Get generic atk errorccode
     * @return int One of the ATK DB_* codes.
     */
    function getAtkDbErrno()
    {
      return "";
    }

    /**
     * Connect to the database.
     * @return int Connection status
     * @abstract
     */
    function connect()
    {
      return true;
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
      return true;
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
      $this->m_query_history[] = $query;
      
      if ($this->m_results[$offset][$limit][$query]) return $this->m_results[$offset][$limit][$query];
      if (!empty($this->m_regex_results))
      {
        foreach ($this->m_regex_results[$offset][$limit] as $regex => $result)
        {
          if (preg_match($regex, $query)) return $result;
        }
      }
      return true;
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
      $res = $this->query($query, $offset, $limit);
      if (is_array($res))
        return $res;
      return array();
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
    function &getInstance($conn="default")
    {
      static $s_instances = array();
      if (!isset($s_instances[$conn]))
      {
        atkdebug("Creating new db instance for connection $conn");
         // include DB stuff
        $s_instances[$conn] = &atknew("atk.db.atkmockdb");
      }
      return $s_instances[$conn];
    }
    
    function setResult($result, $query, $offset=-1, $limit=-1)
    {
      $this->m_results[$offset][$limit][$query] = $result;
    }
    
    function setRegexResult($result, $regex, $offset=-1, $limit=-1)
    {
      $this->m_regex_results[$offset][$limit][$regex] = $result;
    }
  }

?>