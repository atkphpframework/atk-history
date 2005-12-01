<?php

  class atkMockQuery extends atkQuery 
  {    
    /**
     * Static factory method. This method returns a new instance of a query
     * object for the current database.
     */
    function &create()
    {      
      $name = "atkmockquery";
      return new $name();
    }
  }

?>