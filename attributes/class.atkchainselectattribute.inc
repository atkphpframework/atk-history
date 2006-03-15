<?php

  useattrib('atkListAttribute');

  class atkChainSelectAttribute extends atkAttribute 
  {
    var $m_attributes=array();
    
    function atkChainSelectAttribute($name)
    {
      $this->atkAttribute($name,AF_HIDE);
    }
    
    function add(&$attribute)
    {
      $this->m_ownerInstance->add($attribute);
      $this->m_attributes[] = &$attribute;
    }
    
    function addToEditArray($mode, &$arr, &$defaults, &$error, $fieldprefix)
    {
      $page = &$this->m_ownerInstance->getPage();
      $page->register_script(atkconfig("atkroot")."atk/javascript/prototype/prototype.js");
      $page->register_script(atkconfig("atkroot")."atk/javascript/scriptaculous/scriptaculous.js");
      $page->register_script(atkconfig("atkroot")."atk/javascript/recordlist.js");
      $page->register_script(atkconfig("atkroot")."atk/keyboard/javascript/keyboardhandler.js");
      $page->register_script(atkconfig("atkroot")."atk/keyboard/javascript/class.atkrlkeylistener.js");
      $page->register_script(atkconfig("atkroot")."atk/javascript/formselect.js"); 
      
      for ($i=0,$_i=count($this->m_attributes);$i<$_i-1;$i++)
      {
        $url = $this->partial_url($this->m_ownerInstance->atkNodeType(), 'partial', $this->m_attributes[$i+1]->m_name);    
        $this->addElementListener($this->m_attributes[$i]->m_name, str_replace('.','_',$this->m_ownerInstance->atknodetype().'_'.$this->m_attributes[$i+1]->m_name), $url);    
      }
    }

    /**
     * Add element listener. Listen for changes on the
     * given element. Replace the data within the
     * replace element with the data retrieved from the
     * given url. All form values are appended to the given url.
     *
     * @param String $elementName
     * @param String $replaceElementName
     * @param String $url
     */
    function addElementListener($elementName, $replaceElementName, $url)
    {
      $url = addslashes($url);

      $code = "
      new Form.Element.EventObserver(
        \$('$elementName'), 
        function(event, value) {
          var url = '$url';
          formaction = this.element.form.atkaction;
          var params = Form.serialize(this.element);
          alert('Form.Element.serialize(this.element): '+Form.Element.serialize(this.element));
          new Ajax.Updater('{$replaceElementName}', url, {method: 'post', parameters: params, evalScripts: true, asynchronous: true });        
        }
      );";     

      $page = &$this->m_ownerInstance->getPage();
      $page->register_loadscript($code);
    }

    /**
     * Generate partial url.
     *
     * @param unknown_type $node
     * @param unknown_type $action
     * @param unknown_type $partial
     * @param unknown_type $params
     * @return unknown
     */
    function partial_url($node, $action, $partial, $params=array())
    {
      if (!is_array($params))
      $params = array();
      $params['atkpartial'] = $partial;
      return session_url(dispatch_url($node, $action, $params), SESSION_DEFAULT);
    }
  
    function addToQuery(){}
  }

?>