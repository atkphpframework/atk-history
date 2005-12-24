<?PHP

  /**
   * atkInterface class
   */
  class atkInterface
  {
    var $m_format = 'XHTML11';
    
    /**
     * Set the format of the GUI
     *
     * @param String $format The format to use
     */
    function setFormat($format)
    {
      $this->m_format = $format;
    }
    
    /**
     * Getter of the format of the GUI
     *
     * @return unknown
     */
    function getFormat()
    {
      return $this->m_format;
    }
  }

?>