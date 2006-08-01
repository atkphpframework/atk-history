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

  /** @internal Include base class */
  require_once(atkconfig("atkroot")."atk/db/class.atkoci8db.inc");

  /**
   * Oracle 9i database driver.
   *
   * This class is based on oci8db, but makes use of some features that were
   * introduced in Oracle 9i. Should work with Oracle 10i databases too.
   *
   * @author Maurice Maas <maurice@ibuildings.nl>
   * @package atk
   * @subpackage db
   *
   */
  class atkOci9Db extends atkOci8Db
  {
    var $m_bindresult = array();

    var $m_type = "oci9";

    // Defaults
    var $m_port = "1521";

    /**
     * Base constructor
     */
    function atkOci9Db()
    {
      return $this->atkOci8Db();
    }

    /**
     * Connect to the database
     * @return int One of the generic DB_ connection statusses.
     */
    function connect()
    {
      /* establish connection to database */
      if (empty($this->m_link_id))
      {
        $connectfunc = (atkconfig("databasepersistent")?"OCIPlogon":"OCILogon");
        atkdebug("Connectiontype: ".$connectfunc);

        if ($this->m_host!="") // host based connect
        {
          $db = "(DESCRIPTION=(ADDRESS_LIST = (ADDRESS = (PROTOCOL = TCP)(HOST = ".$this->m_host.")(PORT = ".$this->m_port.")))(CONNECT_DATA=(SID=".$this->m_database.")))";
        }
        else
        {
          $db = $this->m_database; // tnsnames based connect
        }
        $this->m_link_id = $connectfunc($this->m_user, $this->m_password, $db);
        if (!$this->m_link_id)
        {
          $err = OCIError();
          $this->halt($this->_translateError($err));
        }

        /* set default date format for Oracle */
        $nls = OCIParse($this->m_link_id, "alter session set nls_date_format='YYYY-MM-DD'");
        $myresult = OCIExecute($nls);
        $error = OCIError($this->m_link_id);
        if ($err["code"]!="")
        {
          $this->halt("Invalid SQL: $query");
          return $this->_translateError($err);
        }

        /* set default timestamp format for Oracle */
        // This should work, because it works in sqlplus
        // Is this a PHP bug
        // Workarround: see class.atkoci9query.inc and class.atkdatetimeattribute.inc
        $nls = OCIParse($this->m_link_id, "alter session set nls_timestamp_format='YYYY-MM-DD HH24:MI:SS'");
        $myresult = OCIExecute($nls);
        $err = OCIError($this->m_link_id);
        if ($err["code"]!="")
        {
          $this->halt("Invalid SQL: $query");
          return $this->_translateError($err);
        }
      }
      return DB_SUCCESS;
    }

    /**
     * Performs a query
     * @param $query the query
     * @param $offset offset in record list
     * @param $limit maximum number of records
     */
    function query($query, $offset=-1, $limit=-1, $bindLobs=array(), $bindVars=array())
    {
      /* limit? */
      if ($offset >= 0 && $limit > 0)
      {
        /* row id's start at 1! */
	      $query="SELECT * FROM (SELECT rownum AS rid, XX.* FROM (".$query.
                ") XX) YY  WHERE YY.rid >= ".($offset+1)." AND YY.rid <= ".($offset + $limit);
      }

      /* query */
      atkdebug("atkoci9db.query(): ".$query);

      /* connect to database */
      if ($this->connect()==DB_SUCCESS)
      {
        /* free old results */
        if ($this->m_query_id)
        {
          @OCIFreeStatement($this->m_query_id);
          $this->m_query_id = 0;
          $this->m_bindresult = array();
        }

        /* query database */
        if (count($bindLobs))
        {
          $i=0;
          foreach($bindLobs as $bind => $value)
          {
            $clobVar = $bind.$i;
            $$clobVar = OCINewDescriptor($this->m_link_id, OCI_D_LOB);
            $i++;
            atkdebug("NewDescriptor for ".$bind);
          }
          $fieldNames = array();
          $bindNames = array();
          $fieldNames = array_keys($bindLobs);
          foreach($fieldNames as $name) $bindNames[] = ":".$name;
          $query .= " RETURNING ".implode(",",$fieldNames)." INTO ".implode(",",$bindNames);
        }

        $this->m_query_id = OCIParse($this->m_link_id, $query);
        if ($this->m_query_id)
        {
          if (count($bindLobs))
          {
            $i=0;
            foreach($bindLobs as $bind => $value)
            {
              $clobVar = $bind.$i;
              OCIBindByName($this->m_query_id, $bind, $$clobVar, -1, OCI_B_CLOB);
              $i++;
              atkdebug("Binding CLOB ".$bind);
            }
          }

          for ($i=0, $_i=count($bindVars);$i<$_i;$i++)
          {
            OCIBindByName($this->m_query_id, $bindVars[$i], $this->m_bindresult[$bindVars[$i]], 32);
          }

          if (!@OCIExecute($this->m_query_id,OCI_DEFAULT))
          {
            /* error in query */
            $error = OCIError($this->m_query_id);
            $this->halt($this->_translateError($error));
            return false;
          }
          else
          {
            if (count($bindLobs))
            {
              $i=0;
              foreach($bindLobs as $bind => $value)
              {
                $clobVar = $bind.$i;
                if (!@$$clobVar->save($value))
                  atkerror("Error bind save: empty value");
                $i++;
              }
            }
          }
          if (count($bindLobs))
          {
            $i=0;
            foreach($bindLobs as $bind => $value)
            {
              $clobVar = $bind.$i;
              OCIFreeDesc($$clobVar);
              $i++;
            }

          }

          $this->m_row = 0;

          return true;
        }
        else
        {
          /* error in query */
          $error = OCIError($this->m_query_id);
          $this->halt("Invalid SQL: $query");
          $this->halt($this->_translateError($error));
          return false;
        }

      }
      return false;
    }
  }
?>