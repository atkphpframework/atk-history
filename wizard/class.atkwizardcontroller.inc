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
    
    function atkWizardController(&$node, &$wizardPanel)
    {
      atkdebug("atkWizardController::constructor() PanelName: " .$wizardPanel->getPanelName()) ;
      // Call base constructor
      $this->atkController(&$node);
      
      $this->m_wizardPanel = &$wizardPanel;      
    }

    /**
     * Returns the form buttons for a certain page.
     *
     * Can be overridden by derived classes to define custom buttons.
     * @param String $mode The action for which the buttons are retrieved.
     * @param array $record The record currently displayed/edited in the form.
     *                      This param can be used to define record specific
     *                      buttons.
     */
    function getFormButtons($mode, $record)
    {
      //global $ATK_VARS;
      
      atkdebug("atkWizardController::getFormButtons()");
      $result = array();

      if ($mode == "add")
      {
        // We post the action as key value in de atkwizardaction var. Therefor
        // we have to convert the atkwizardaction value in atkWizard::start().        
        if(!$this->m_wizardPanel->isFinishPanel())
        {
          if($this->m_node->m_action != 'admin')
          {
            $result[] = '<input type="submit" class="btn_next" name="atkwizardaction[next]" value="'.atktext("next", "atk").'">';
          }
          else 
          {
            $result[] = '<input type="submit" class="btn_next" name="atkwizardaction[save_addnew]" value="'.atktext("save_addnew", "atk").'">';
            $result[] = '<input type="submit" class="btn_next" name="atkwizardaction[save_next]" value="'.atktext("save_next", "atk").'">';
          }
        }
        else         
        {          
          if($this->m_node->m_action == 'admin')
          {          
            $result[] = '<input type="submit" class="btn_next" name="atkwizardaction[save_addnew]" value="'.atktext("save_addnew", "atk").'">';
          }            
          $result[] = '<input type="submit" class="btn_next" name="atkwizardaction[finish]" value="'.atktext("finish", "atk").'">';
        }
          
        // if action is admin, we don't show the cancelbutton or the add next button
        $result[] = '<input type="submit" class="btn_cancel" name="atkwizardcancel" value="'.atktext("cancel", "atk").'">';
      }
      else
      {
        $result = parent::getFormButtons($mode, $record);
      }

      return $result;
    }    
    
  }

?>