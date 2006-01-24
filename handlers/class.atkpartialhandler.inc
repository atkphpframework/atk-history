<?php

  class atkPartialHandler extends atkActionHandler 
  {
    function action_partial()
    {
      $attr = $this->m_postvars["atkpartial"];
      $method = "partial_$attr";
      
      if (method_exists($this->m_node, $method))
        $output = $this->m_node->$method();
      else 
      {
        $output = $this->partial_attribute($attr);
      }
      $page = &$this->m_node->getPage();
      $page->addContent($output);
      return true;
    }
    
    
    function partial_attribute($attr)
    {
      $attribute = &$this->m_node->getAttribute($attr);
      return $attribute->edit();
    }    
  }

?>