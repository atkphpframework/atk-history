<?php

  /**
   * Oracle8 specific implementations of the aktDDL utility class.
   *
   * @author Joop van de Wege <JoopvandeWege@mococo.nl>
   * @version $Revistion$
   *
   * $Id$
   */

  class atkoci8ddl extends atkDDL
  {
    /**
     * Convert an ATK generic datatype to a database specific type.
     * 
     * This function will be overrided by the database specific subclasses of
     * atkDb.
     * Note: in all derived subclasses, the following types *must* be 
     * supported: number, decimal, string, date, text, datetime, time, 
     * boolean.
     * If the database does not have a proper field type, consider using
     * a varchar or number to store the value.
     * 
     * @param $generictype  The datatype to convert.
     */
    function getType($generictype)
    {      
      switch($generictype)
      {
        case "number": 
          return "NUMBER";
        case "decimal":
          return "NUMBER";
        case "string":
          return "VARCHAR2";          
        case "date":
          return "DATE";
        case "text":
          return "VARCHAR2(4000)"; // @TODO JvdW 10-01-2004 CLOB's can't be used in 'select DISTINCT ' 
        case "datetime":
          return "DATE";
        case "time":
          return "DATE";          
        case "boolean":        
          return "NUMBER(1,0)"; // size is added fixed. (because a boolean has no size of its own)          
      }
      return ""; // in case we have an unsupported type.      
    }

  }
    /**
     * Convert an database specific type to an ATK generic datatype.
     * 
     * This function will be overrided by the database specific subclasses of
     * atkDb.     
     * 
     * @param $type  The database specific datatype to convert.
     */
    function getGenericType($type)
    {      
      $type = strtolower($type);
      switch($type)
      {
        case "number": 
          return "number";        
        case "char":
        case "varchar2":
          return "string";          
        case "date":
          return "date";
        case "clob": // @TODO JvdW 10-01-2004 should it be 'case VARCHAR2(4000):' ?? 
          return "text";
        case "date":
          return "date";                
      }
      return ""; // in case we have an unsupported type.      
    }

?>