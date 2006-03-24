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
  
 atkimport("atk.wizard.atkwizardactionloader");
 
/**
 * atkWizard class which is capable of using atknodes
 *
 * This class makes the distinction between update/save and 
 * navigation actions and respondis correspondingly.
 *
 * @author maurice <maurice@ibuildings.nl>
 * @package atk
 *
 */
  class atkWizard
  {
    /**
     * Array of panels 
     * First item is first to show in wizard
     *      
     * @access private
     * @var array
     */
    var $m_panelList = array();
    var $m_panelIndex = array();
    
    /**
     * Key-value array which will be added as hidden value in edit/add forms
     *      
     * @access private
     * @var array
     */

    var $m_hiddenParams = array();
    
    /**
     * Key-value array which will be url when redirecting to a new page
     *      
     * @access private
     * @var array
     */
    var $m_redirectParams = array();
    
    /**
     * Index of panel which is currently shown or processed
     *      
     * @access private
     * @var array
     */
    var $m_currentPanelIndex;
       
    /**
     * return the html for the current panel or let the node display itself
     *      
     * @access public
     * @var boolean
     */
    var $m_returnOutput;
    
    /**
     * We use a reference to ourselfs because we pass data to the wizardpanel
     * which has a reference to its wizard parent.
     *      
     * @access private
     * @var object
     */
    var $m_self;
    
    /**
     * Constructor
     *
     * @param boolean $returnOutput Indicate if this wizard should return output or not
     * @return atkWizard
     */    
    function atkWizard($returnOutput=false)
    {
      $this->m_currentPanelIndex = atkGetPostVar("atkwizardpanelindex");      
      $this->m_returnOutput = $returnOutput;
      $this->m_self = &$this;
    }
    
    /**
     * Add wizardpanel to the wizard. Only objects of type
     * atkWizardPanel can be added. The order of additions
     * to the wizard determines order in which the panels are
     * shown.
     *
     * @param object $atkWizardPanel
     */
    function addPanel(&$atkWizardPanel)
    {
      $this->m_panelList[] = &$atkWizardPanel;
      $this->m_panelIndex[$atkWizardPanel->getPanelName()] = count($this->m_panelList)-1;
    }
    
    function excecute()    
    {
      atk_var_dump(atkGetPostVar(), "Postvars");
      
      $atkwizardaction = AtkWizardActionLoader::getWizardAction(atkGetPostVar("atkwizardaction"));           
      if(atkGetPostVar("atkaction") == 'save' || atkGetPostVar("atkaction") == "update")
      {
        //save form
        return $this->save($atkwizardaction);               
      }
      elseif($atkwizardaction == 'finish')
      {
        return $this->finish();
      }
      else 
      {
        return $this->dispatch($atkwizardaction);
      }  
    }        
    
    function dispatch($atkWizardaction)
    {
      if($atkWizardaction == 'next')
      {        
        $wizardPanel = $this->m_panelList[$this->m_currentPanelIndex+1];        
        $this->setHiddenParam("atkwizardpanelindex", $this->m_currentPanelIndex+1); 
      }
      elseif($atkWizardaction == 'save_addnew')
      {        
        //We saved the panel but we want to show the same panel again.
        $wizardPanel = $this->m_panelList[$this->m_currentPanelIndex];        
        $this->setHiddenParam("atkwizardpanelindex", $this->m_currentPanelIndex); 
      }
      else 
      {
        //first panel     
        $wizardPanel = $this->m_panelList[0];        
        $this->setHiddenParam("atkwizardpanelindex", 0);                 
      }
      return $wizardPanel->dispatchForm($this->m_hiddenParams, $this->m_returnOutput);
    }
    
    //Rename to more generic name
    function save($atkwizardaction)
    {
      // We use a reference to ourselfs because we pass data to the wizardpanel
      // which has a reference to its wizard parent.
      $this->m_self->setRedirectParam("atkwizardpanelindex", $this->m_currentPanelIndex);
      $this->m_self->setRedirectParam("atkwizardaction", $atkwizardaction);
      
      $currentWizardPanel = $this->getCurrentPanel();
      atkdebug("atkWizard::save panel: " . $currentWizardPanel->getPanelName());
      return $currentWizardPanel->save();
    }
    
    /**
     * Finish the wizard
     * 
     * This function can be overriden in the your extended wizard class. 
     * 
     * TODO/FIXME It would be even more cool if wizardpanels could listen to
     * the finish action and be notified of this action/event.
     *
     */
    function finish()
    {
      return "The wizard has finished.";
    }    
    
    function setHiddenParam($key, $value)
    {
      $this->m_hiddenParams[$key] = $value;
    }

    function getHiddenParam()
    {
      return $this->m_hiddenParams;
    }

    
    function setRedirectParam($key, $value)
    {
      $this->m_redirectParams[$key] = $value;
    }
    
    function getCurrentPanel()
    {
      
      $currentWizardPanel = $this->m_panelList[$this->m_currentPanelIndex];      
      if(!is_object($currentWizardPanel)) 
      {
        atkerror("Panel could not be crated. Non existing panel index: " . $this->m_currentPanelIndex);
        return NULL;
      }
      return $currentWizardPanel;
    }
    
  }
?>