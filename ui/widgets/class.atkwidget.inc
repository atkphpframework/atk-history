<?PHP

  /**
   * Abstract baseclass for ATK widgets
   *
   * Implements some custom functionality and defines some methods that
   * derived classes should override.
   */
  class atkWidget
  {    
    /**
     * Static factory method for creating a new atkWidget instance. This static
     * method will determine format (xthml strict 1.1, html 3.2, xml, etc) and
     * the widget to use, (select, inputbox) and returns 
     * a correctly instantiated class.
     */
    function &create($widget)
    {
      atkimport('atk.ui.atkinterface');
      $gui = atkInterface::getFormat();
      $classname = "atk".$gui.$widget."Widget";
      return atknew($classname);
    }
    
    /**
     * Method by which a widget can be bound to a source of data.
     *
     * @param Object $attribute
     * @return bool Wether we succeeded in binding with the object
     */
    function bind($object)
    {
      return false;
    }
  }

?>