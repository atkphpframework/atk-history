<?php

/**
 * ATK recordlist container.
 *
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkRecordListContainer
{
  var $m_name   = NULL;
  var $m_form   = NULL;
  var $m_node   = NULL;
  var $m_index  = NULL;  
  var $m_action = NULL;
  var $m_flags  = NULL;
  var $m_mraSelectionMode = NULL;
  
  var $m_records = array();
    
  var $m_actions  = NULL;  
  var $m_excludes = array();
  
  var $m_header = "";
  var $m_footer = "";
  
  var $m_navigator = "atk.utils.atknavigator";
  var $m_recordlist = "atk.recordlist.atkrecordlist";
  
  function setName($name)
  {
    $this->m_name = $name;
  }
  
  function setForm($form)
  {
    $this->m_form = $form;
  }
  
  function setNode(&$node)
  {
    $this->m_node = &$node;
  }
  
  function setIndex($attr)
  {
    $this->m_index = $attr;
  }
  
  function setAction($action)
  {
    $this->m_action = $action;
  }
  
  function setFlags($flags)
  {
    $this->m_flags = $flags;
  }
  
  function setMRASelectionMode($mode)
  {
    $this->m_mraSelectionMode = $mode;
  }

  function setRecords($records)
  {
    $this->m_records = $records;
  }  
  
  function setActions($actions)
  {
    $this->m_actions = $actions;
  }
  
  function setExcludes($excludes)
  {
    $this->m_excludes = $excludes;
  }
  
  function setHeader($header)
  {
    $this->m_header = $header;
  }
  
  function setFooter($footer)
  {
    $this->m_footer = $footer;
  }
  
  function setNavigator($class)
  {
    $this->m_navigator = $class;
  }
  
  function setRecordList($class)
  {
    $this->m_recordlist = $class;
  }  
  
  function &_createNavigator()
  {
    return atknew($this->m_navigator);
  }
  
  function &_createRecordList()
  {
    return atknew($this->m_recordlist);    
  }
  
  function render($partial=false)
  {
    $preAction = $this->m_node->m_action;
    $this->m_node->m_action = $this->m_action;
    
    $params = array();
    
    /* @var $navigator atkNavigator */
    /* @var $recordlist atkRecordList */
    $navigator = $this->_createNavigator();
    $recordlist = &$this->_createRecordList();
    
    $count = count($this->m_records);
    
    $name = $this->m_name;
    if ($name === NULL)
      $name = "rl_".getUniqueId(get_class($recordlist));           
    
    $actions = $this->m_actions;
    if ($actions === NULL)
      $actions = $this->m_node->defaultActions($this->m_action);
      
    $flags = $this->m_flags;
    if ($flags === NULL)
      $flags = atkRecordList::convertFlags($this->m_node->getFlags());
    
    $index = $this->m_index;  
    if ($this->m_index === NULL)
      $index = $this->m_node->m_index;
      
    $mraSelectionMode = $this->m_mraSelectionMode;
    if ($mraSelectionMode === NULL)
      $mraSelectionMode = $this->m_node->m_mraSelectionMode;
    
    if ($index != FALSE && !empty($index))
      $params['index'] = $navigator->buildIndex($this->m_node, $count > 0 ? $records[0][$index] : '');
      
    $navigation = $navigator->getNavigationUrls($this->m_node, $this->m_action, '', $name);
    if ($navigation)
    {
      if (array_key_exists('index', $params))
        $params['index'] .= '<br />';
      
      $params['navbar'] = $navigator->buildNavigation($this->m_node, $this->m_action, $navigation, false, $name);
    }
    
    $params['list'] = $recordlist->render($this->m_node, $this->m_records, $actions, $flags, $this->m_excludes, '', $navigation, '', $name, $mraSelectionMode);
    $params['header'] = $this->m_header;
    $params['footer'] = $this->m_footer;

    $ui = &atkUI::getInstance();
    $output = $ui->renderList($this->m_action, $params);
        
    if (!$partial)
      $output = "<div id=\"{$name}_container\">{$output}</div>";
      
    $this->m_node->m_action = $preAction;
      
    return $output;
  }
}
?>