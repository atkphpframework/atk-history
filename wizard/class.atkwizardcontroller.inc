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
 
atkimport("atk.atkcontroller");
 
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
  class atkWizardController extends atkController
  {
    var $m_wizardPanel = null;
    
    var $m_page_flags = HTML_ALL;
    
    function atkWizardController(&$node, &$wizardPanel)
    {
      atkdebug("atkWizardController::constructor() PanelName: " .$wizardPanel->getPanelName()) ;
      // Call base constructor
      $this->atkController(&$node);
      
      $this->m_wizardPanel = &$wizardPanel;      
    }

    /**
     * This is the wrapper method for all http requests on a node.
     *
     * The method looks at the atkaction from the postvars and determines what
     * should be done. If possible, it instantiates actionHandlers for
     * handling the actual action.
     *
     * @param array $postvars The request variables for the node.
     * @param boolean $fullpage If true (default), the dispatch function
     *                          renders a complete html page, including
     *                          <html><head><body> etc. If you set this to
     *                          false, plain output is generated. Warning: If
     *                          you set this to false, you are responsible for
     *                          loading all the required scripts, stylesheets
     *                          etc. (When $fullpage is true, these are
     *                          automatically included in the page header.
     */
    function dispatch($postvars, $fullpage=true)
    {
      // Use invoke to be backwards compatible with overrides 
      // of loadDispatchPage in atknode.
      $this->invoke("loadDispatchPage", $postvars);
      //$this->loadDispatchPage($postvars);

      $ui = &$this->m_node->getUi();
      $page = &$this->m_node->getPage();
      atkdebug("atkWizardController::dispatch() flags" . $this->m_page_flags);
      $screen = $page->render(atktext('app_shorttitle')." - ".$ui->title($this->m_node->m_module, $this->m_node->m_type, $this->m_node->m_postvars['atkaction'], true), $this->m_page_flags);

      if(!$this->m_return_output)
      {
        $output = &atkOutput::getInstance();
        $output->output($screen);
      }

      // This is the end of all things for this page..
      // so we clean up some resources..
      $db = &$this->m_node->getDb();
      if (is_object($db)) $db->disconnect();
      atkdebug("disconnected from the database");
      
      if($this->m_return_output)
      {
        atkdebug("atkController::dispatch Return output ");
        return $screen;
      }
    }
    
    /**
     * Returns the form buttons for a certain page.
     *
     * Can be overridden by derived classes to define custom buttons.
     * @param String $action The action for which the buttons are retrieved.
     * @param array $record The record currently displayed/edited in the form.
     *                      This param can be used to define record specific
     *                      buttons.
     */
    function getFormButtons($action, $record)
    {
      $result = array();
      
      if ($this->m_wizardPanel->m_wizard->m_mode == WIZARD_MODE_ADD)
      {
        
        // We post the action as key value in de atkwizardaction var. Therefor
        // we have to convert the atkwizardaction value in atkWizard::start().                
        if($this->m_node->m_action != 'admin')
        {
          //if we explicitly don't want the finish button we set a hidden var to post the atkwizardaction
          atkdebug("FINISH_BUTTON ".$this->m_wizardPanel->showFinishButton());
          atkdebug("FINISH_BUTTON_DONT_SHOW ".($this->m_wizardPanel->showFinishButton() == FINISH_BUTTON_DONT_SHOW ? "TRUE" : "FALSE") ." isfinishpanel " . ($this->m_wizardPanel->isFinishPanel() ? "TRUE" : "FALSE"));
          if(($this->m_wizardPanel->showFinishButton() == FINISH_BUTTON_DONT_SHOW) && $this->m_wizardPanel->isFinishPanel())
            $atkwizardaction = "finish";
          else 
            $atkwizardaction = "next";
          
          if($this->showFinishButton())
            $result[] = '<input type="submit" class="btn_next" name="atkwizardaction[finish]" value="'.atktext("finish", "atk").'">';
          else 
            $result[] = '<input type="submit" class="btn_next" name="atkwizardaction['.$atkwizardaction.']" value="'.atktext("next", "atk").'">';
        }
        else 
        {
          //if we explicitly don't want the finish button we set a hidden var to post the atkwizardaction
          if($this->m_wizardPanel->showFinishButton() == FINISH_BUTTON_DONT_SHOW && $this->m_wizardPanel->isFinishPanel())
            $atkwizardaction = "finish";
          else 
            $atkwizardaction = "saveandnext";
          
          $result[] = '<input type="submit" class="btn_next" name="atkwizardaction[saveandaddnew]" value="'.atktext("saveandaddnew", "atk").'">';
          if($this->showFinishButton())
            $result[] = '<input type="submit" class="btn_next" name="atkwizardaction[finish]" value="'.atktext("finish", "atk").'">';
          else
            $result[] = '<input type="submit" class="btn_next" name="atkwizardaction['.$atkwizardaction.']" value="'.atktext("saveandnext", "atk").'">';
        }
        
        $result[] = '<input type="submit" class="btn_cancel" name="atkwizardcancel" value="'.atktext("cancel", "atk").'">';
      }
      elseif ($this->m_wizardPanel->m_wizard->m_mode == WIZARD_MODE_EDIT)
      {
        // We post the action as key value in de atkwizardaction var. Therefor
        // we have to convert the atkwizardaction value in atkWizard::start().        
        $result[] = '<input type="submit" class="btn_save" name="atknoclose" value="'.atktext("save", "atk").'">';
        //$result[] = '<input type="submit" class="btn_next" name="atkwizardaction[saveandnext]" value="'.atktext("saveandnext", "atk").'">';                //$result[] = '<input type="submit" class="btn_save" name="atknoclose" value="'.atktext("save", "atk").'">';        
        $result[] = '<input type="submit" class="btn_next" name="atkwizardaction[finish]" value="'.atktext("finish", "atk").'">';          
        $result[] = '<input type="submit" class="btn_cancel" name="atkcancel" value="'.atktext("cancel", "atk").'">';
      }
      else
      {
        $result = parent::getFormButtons($action, $record);
      }

      return $result;
    }    
    
    function setPageFlags($flag)
    {
      $this->m_page_flags = $flag;
    }

    /**
     * Determine if this panel should show a finish button
     */
    function showFinishButton()
    {
      return (($this->m_wizardPanel->showFinishButton() == FINISH_BUTTON_SHOW) ||
               ($this->m_wizardPanel->showFinishButton() == FINISH_BUTTON_DEFAULT && $this->m_wizardPanel->isFinishPanel()));
    }    
    
  }
  
  

?>