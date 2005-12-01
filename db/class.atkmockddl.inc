<?php

class atkMockDDL extends atkDDL
{
  function atkMockDDL()
  {
  }

  /**
   * Static factory method for creating a new atkDDL instance. This static
   * method will determine the database type (mysql, oci, etc) and
   * instantiate the correct DDL class.
   */
  function &create($database=NULL)
  {
    $database = "mock";

    $filename = atkconfig("atkroot")."atk/db/class.atk".$database."ddl.inc";
    if (file_exists($filename))
    {
      include_once($filename);
      $classname = "atk".$database."DDL";
      return new $classname;
    }
    else
    {
      atkerror("atkddl::create: No support for $database!");
    }
    return NULL;
  }
}

?>