<?php
/** 
 * This file is part of the Achievo ATK distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package atk
 *
 * @author maurice <maurice@ibuildings.nl>
 *
 * @copyright (c) 2006 Ibuildings.nl BV
 * @license see doc/LICENSE
 *
 * @version $Revision$
 * $Id$
 */

atkimport("atk.handlers.atkactionhandler");
atkimport("atk.handlers.atkaddhandler");
atkimport("atk.handlers.atksavehandler");
 
/**
 * Wizardpanel which binds to a atknodes
 *
 * Full description of the class
 *
 * @author maurice <maurice@ibuildings.nl>
 * @package atk
 *
 */

  class atkWizardPanel
  {
    var $m_wizard;
    var $m_panelName;    
    
    var $m_nodeName;
    var $m_node;
    
    var $m_listeners;
    
    var $m_redirectParams = array();
    
    var $m_defaultAction;
    
    /**
     * atkWizardPanel constructor
     *
     * We don't construct the node here for performance reasons (if any?). We
     * probably don't need the node in most pageloads because we only use one
     * wizardpanel at a time.
     * 
     * @param unknown_type $wizard
     * @param unknown_type $panelName
     * @param unknown_type $nodeName
     * @return atkWizardPanel
     */    
    function atkWizardPanel(&$wizard, $panelName, $nodeName, $defaultAction="")
    {
      $this->m_wizard = &$wizard;
      $this->m_panelName = $panelName;
      $this->m_nodeName = $nodeName;           
      $this->m_defaultAction = $defaultAction;
    }
    
    function getPanelNode()
    {
      if (!is_object($this->m_node))
      {
        atkdebug("atkWizardPanel::getPanelNode() create node. Node name: " . $this->m_nodeName);
        $this->m_node = &getNode($this->m_nodeName);          
        if(!is_object($this->m_node)) 
        {
          atkerror("atkWizardPanel::getPanelNode() Node could not be created. Node name: " . $this->m_nodeName);
          return;
        }
        //Add listeners to node
        for($i=0, $_i=count($this->m_listeners); $i<$_i; $i++)
        {
          $this->m_node->addListener($this->m_listeners[$i]);
        }
        
        //todo: not sure if this needed
        //Add postvars to node manually 
        $this->m_node->m_postvars = getPostVar();
        
        //All nodes should return the output and not try to fill the screen themselves
        $controller = &$this->m_node->getController();
        $controller->setReturnOutput(true);
        atkdebug("atkController m_return_output: " . ($controller->m_return_output ? "TRUE" : "False"));

      }
      return $this->m_node;  
    }
    
    function getPanelName()
    {
      return $this->m_panelName;
    }
    
    /**
     * Add an atkActionListener to the atkWizardPanel.
     * 
     * Listeners are added to the atkNode when the
     * node is created.
     *
     * @param atkActionListener $listener
     */
    function addListener(&$listener)
    {
      $this->m_listeners[] = $listener;
    }

   
    function dispatchForm($params, $returnOutput)
    {    	
      atkdebug("atkWizardPanel::getAddForm() panel=".$this->getPanelName() . " atknode=".$this->m_nodeName);
      $node = &$this->getPanelNode();         
      if(!is_object($node)) return;
      
      $controller = &$node->getController();                
      
      atk_var_dump($params, "hiddenparams");
      if(count($params))
      {
        foreach($params as $hiddenVarName => $hiddenVarValue)
        {          
          $controller->setHiddenVar($hiddenVarName, $hiddenVarValue);
        }
      }
      
	    global $ATK_VARS;
	    atkdebug("atkaction set " .$ATK_VARS['atkaction']);
	    if(!isset($ATK_VARS['atkaction']) || $ATK_VARS['atkaction'] == "")
	    {
	      atkdebug("default action " .$this->m_defaultAction);
	      if(!isset($this->m_defaultAction) || $this->m_defaultAction == "")
	        $ATK_VARS['atkaction'] = 'add';
	      else 
	        $ATK_VARS['atkaction'] = $this->m_defaultAction;
	    }

	    //If atk renders the page itself the return value is empty
	    $output = $controller->dispatch($ATK_VARS, false);
        
      return $output;
    }    
    
    function save()
    {
      atkdebug("atkWizardPanel::save node: " . $this->m_nodeName);
      $node = $this->getPanelNode();         
      if(!is_object($node)) return;
      
      $controller = &$node->getController();
      
      $hiddenVars = $this->m_wizard->m_redirectParams;
      if(count($hiddenVars))
      {
        foreach($hiddenVars as $hiddenVarName => $hiddenVarValue)
        {          
          atkdebug("Add $hiddenVarName value $hiddenVarValue");          
          $controller->setHiddenVar($hiddenVarName, $hiddenVarValue);
        }
      }

      if(getPostVar("atkaction") == "save")
      {
        $saveHandler = new atkWizardSavehandler();
        registerNodeHandler($node->m_type, "save",  $saveHandler);
      }
      
      //Save, on succes dispatch() will redirect
      //else it will return an 'add form' with error messages
      return $controller->dispatch(getPostVar(), false);
    }

  }
    

  class atkWizardSavehandler extends atkSaveHandler
  {  
    function createRedirectLocation($record)
    {
      atkdebug("atkWizardSaveHandler::createRedirectLocation " . $this->m_next_action);
      //return session_url(dispatch_url($this->m_next_node, $this->m_next_action, $this->m_params, $this->m_redirect_file),SESSION_REPLACE);
      $url = $_SERVER["PHP_SELF"].'?atkwizardaction=next';      
      $controller = &$this->m_node->getController();
      atk_var_dump($controller->getHiddenVars(), "this->m_node->getHiddenVars()");
      if ($controller->getHiddenVars()!=""&&is_array($controller->getHiddenVars()))
      {
        foreach ($controller->getHiddenVars() as $key => $value)
        {
          $url .= "&$key=".rawurlencode($value);
        }
      }      
      return session_url($url, SESSION_DEFAULT);
      
      //return parent::createRedirectLocation($record);
    }  
  }
  
?>