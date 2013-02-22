/*
 *  Yarp Modules Manager
 *  Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 * 
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#ifndef _APPLICATION_WINDOW_H_
#define _APPLICATION_WINDOW_H_

#include <gtkmm.h>
#include <goocanvasmm.h>
#include <goocanvasgrid.h>
#include <vector>
#include <map>

#include <yarp/os/Network.h>
#include <yarp/os/Property.h>
#include <yarp/os/ConstString.h>
#include <yarp/os/Time.h>

#include "manager.h"
#include "module_model.h"
#include "port_model.h"
#include "module_property_window.h"
#include "app_property_window.h"
#include "con_property_window.h"


class MainWindow;


class ApplicationWindow: public Gtk::Frame
{
public:
    ApplicationWindow(const char* szAppName, Manager* lazy, 
        yarp::os::Property* config, MainWindow* parent, bool grid=true, bool snap=true);
    virtual ~ApplicationWindow();
    const char* getApplicationName(void) { return m_strAppName.c_str(); }
    Application* getApplication(void) { return manager.getKnowledgeBase()->getApplication(); }

    bool onClose(void);    
    bool onRefresh(void);
    bool onSelectAll(void);
    bool onSave(const char* szfilename=NULL);
    void onDelete(void);
    void onCopy(void);
    void onPaste(void);
    void onMenuInsertSrcPort();
    void onMenuInsertDestPort();
    void onRotateRight();
    void onRotateLeft();
    void onMenuWindowProperty(bool active);
    
    void onZoomIn(void) {
        double scale = m_Canvas->get_scale();
        scale = (scale>=1.6) ? scale: scale+0.15;
        m_Canvas->set_scale(scale);
    }
    void onZoomOut(void) { 
        double scale = m_Canvas->get_scale();
        scale = (scale<=0.4) ? scale: scale-0.15;
        /*
        Goocanvas::Bounds bc;
        m_Canvas->get_bounds(bc);
        int w = (bc.get_x2() - bc.get_x1()) * scale;
        int h = (bc.get_y2() - bc.get_y1()) * scale;        
        if((w>get_width()) && (h>get_height()))
        */
        m_Canvas->set_scale(scale);
    }

    void onZoomReset(void) { 
        m_Canvas->set_scale(1.0);
    }

    void onGrid(bool grid) {
        m_showGrid = grid;
        if(!m_showGrid)        
            g_object_set(m_Grid, "visibility", GOO_CANVAS_ITEM_HIDDEN, NULL);
        else
            g_object_set(m_Grid, "visibility", GOO_CANVAS_ITEM_VISIBLE, NULL);
        m_snapToGrid = (m_snapToGrid && m_showGrid);
    }

    void onSnapToGrid(bool snap) {
       m_snapToGrid = (snap && m_showGrid);
    }

    void onViewLabel(bool label);

    bool onExportGraph(void);
    void onTabCloseRequest();
    void releaseApplication(void);
    MainWindow* getMainWindow(void) { return m_pParent; }
    Glib::RefPtr<Goocanvas::ItemModel> getRootModel(void) { return root; }

    void onUpdateModuleProperty(Module* module);
    void onUpdateApplicationProperty(Application* application);
    void onUpdateConnectionProperty(Glib::RefPtr<ArrowModel> &arrow);

    void setModified(void) {m_bModified = true; }

public: 
    Goocanvas::Canvas* m_Canvas ;
    GooCanvasItemModel *m_Grid;
    bool m_snapToGrid;
    bool m_showGrid;
    bool m_showLabel;
    Manager manager;

protected:
    
    Glib::RefPtr<Gtk::ListStore> m_refListStore; //The Tree Model.
    Gtk::TreeView m_TreeView; //The Tree View.

    Gtk::VBox m_VBox;
    Gtk::HPaned m_HPaned;
    Gtk::Notebook m_RightTab;    

    Gtk::ScrolledWindow m_ScrollView; 
    Glib::RefPtr<Goocanvas::ItemModel> root;
    void on_size_allocate(Gtk::Allocation& allocation);
    bool on_key_press_event(GdkEventKey* event);
    
    int dummy_h;
    int dummy_w;

private:
    MainWindow* m_pParent;
    string m_strAppName;
    bool m_bModified;
    yarp::os::Property* m_pConfig;
    ModulePropertyWindow* modPropertyWindow;
    ApplicationPropertyWindow* appPropertyWindow;
    ConnectionPropertyWindow* conPropertyWindow;

    Glib::RefPtr<Goocanvas::PolylineModel> m_connector;
    Glib::RefPtr<Goocanvas::RectModel> m_selector;
    Glib::RefPtr<PortModel> sourcePort;

    std::vector<ModuleModel*> copiedItems;

    GdkPixbuf* pixV;
    GdkPixbuf* pixH;
    double _x;
    double _y;
private:
    void createWidgets(void);
    void setupSignals(void);        
    void updateApplicationWindow(void);
    void prepareManagerFrom(Manager* lazy, const char* szAppName);
    void reportErrors(void);
    void deleteSelectedArrows(void);
    void setSelected(void);
    int countSelected(void);
    void on_item_created(const Glib::RefPtr<Goocanvas::Item>& item, const Glib::RefPtr<Goocanvas::ItemModel>& model) ;
    bool on_item_button_press_event(const Glib::RefPtr<Goocanvas::Item>& item, GdkEventButton* event) ;
    bool on_item_button_release_event(const Glib::RefPtr<Goocanvas::Item>& item, GdkEventButton* event) ;
    bool on_item_motion_notify_event(const Glib::RefPtr<Goocanvas::Item>& item, GdkEventMotion* event) ;
    bool on_item_focus_in_event(const Glib::RefPtr<Goocanvas::Item>& view, GdkEventFocus* event) ;
    bool on_item_focus_out_event(const Glib::RefPtr<Goocanvas::Item>& view, GdkEventFocus* event) ;
    bool on_item_enter_notify_event(const Glib::RefPtr<Goocanvas::Item>& target, GdkEventCrossing* event);
    bool on_item_leave_notify_event(const Glib::RefPtr<Goocanvas::Item>& target, GdkEventCrossing* event);
    void onDragDataReceived(const Glib::RefPtr<Gdk::DragContext>& context, 
                            int x, int y, const Gtk::SelectionData& data, 
                            guint info, guint time);
    bool onDragDrop(const Glib::RefPtr<Gdk::DragContext>& drag_context, 
                    int x, int y, guint timestamp);
    bool onDragMotion(const Glib::RefPtr<Gdk::DragContext>& drag_context, 
                    int x, int y, guint timestamp);

    Connection* findConnection( CnnContainer& connections, const char* szPort, bool from);
    InputData* findInputData(Connection* cnn,  ModulePContainer &modules);
    Glib::RefPtr<PortModel> findModelFromOutput(OutputData* output);
    Glib::RefPtr<PortModel> findModelFromInput(InputData* input);
    void findInputOutputData(Connection& cnn,  ModulePContainer &modules,
                                            InputData* &input_, OutputData* &output_);

};

#endif //_APPLICATION_WINDOW_H_
