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
atkimport("atk.wizard.atkwizardcontroller");
 
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
    /**
     * Reference to the parent wizard
     *
     * @var object reference
     */
    var $m_wizard;
    
    /**
     * Panel name
     *
     * @var string
     */
    var $m_panelName;    
    
    /**
     * Name representation of the atkNode
     *
     * @var string
     */
    var $m_nodeName;
    
    /**
     * Reference to the atkNode
     *
     * @var object reference
     */
    var $m_node;
    
    /**
     * Array of Actionlisteners which listen
     * to the atkNode
     *
     * @var array
     */
    var $m_listeners;
    
    /**
     * Array of ActionHandlers which are binded
     * to the atkNode
     *
     * @var array
     */
    var $m_actionHandlers = array();
       
    /**
     * The default atkNode action for this wizardpanel
     *
     * @var unknown_type
     */
    var $m_defaultAction;
    
    /**
     * 
     *
     * @var unknown_type
     */    
    var $m_finishPanel = false;
    
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
    function atkWizardPanel(&$wizard, $panelName, $nodeName, $defaultAction="", $finishPanel=false)
    {
      $this->m_wizard = &$wizard;
      $this->m_panelName = $panelName;
      $this->m_nodeName = $nodeName;           
      $this->m_defaultAction = $defaultAction;
      $this->m_finishPanel = $finishPanel;
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

        //Set wizard specific controller
        atkdebug("atkWizardPanel::getPanelNode() Register controller for. Node: " . getNodeType($this->m_nodeName));
        registerNodeController(getNodeType($this->m_nodeName), new atkWizardController($this->m_node, $this));


        //Add listeners to node
        for($i=0, $_i=count($this->m_listeners); $i<$_i; $i++)
        {
          $this->m_node->addListener($this->m_listeners[$i]);
        }

        //Add actionhandlers to node
        for($i=0, $_i=count($this->m_actionHandlers); $i<$_i; $i++)
        {          
          $handlerName = $this->m_actionHandlers[$i]['name'];
          $handlerAction = $this->m_actionHandlers[$i]['action'];
          
          atkdebug("atkWizardPanel::getPanelNode() registerNodeHandler for. Node: " . getNodeType($this->m_nodeName). " name: ".$handlerName. " action ". $handlerAction);
          
          $handler = new $handlerName();
          registerNodeHandler($this->m_node->m_type, $handlerAction, $handler);
        }
                
        //todo: not sure if this needed
        //Add postvars to node manually 
        $this->m_node->m_postvars = atkGetPostVar();
        
        
        //All nodes should return the output and not try to fill the screen themselves
        $controller = &$this->m_node->getController();
        $controller->setReturnOutput(true);
        atkdebug("atkController m_return_output: " . ($controller->m_return_output ? "TRUE" : "False"));
        
        //We set how we want the atk page to be returned
        atkdebug("getPanelNode flags" . $this->m_wizard->m_page_flags);
        $controller->setPageFlags($this->m_wizard->m_page_flags);

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

    
    /**
     * Add an atkActionHandler to the atkWizardPanel.
     * 
     * Handlers are created and added to the atkNode when the
     * node is created.
     *
     * @param atkActionHandler $listener
     */
    function addActionHandler($handlerName, $handlerAction)
    {
      $this->m_actionHandlers[] = array("name" => $handlerName, "action" => $handlerAction);
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
    
    //Rename to more generic name
    function save()
    {
      atkdebug("atkWizardPanel::save node: " . $this->m_nodeName);
      $node = &$this->getPanelNode();         
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
      
      //Save, on succes dispatch() will redirect
      //else it will return an 'add form' with error messages
      return $controller->dispatch(atkGetPostVar(), false);
    }
    
    function isFinishPanel()
    {
      return $this->m_finishPanel;
    }

  }   
  
?>