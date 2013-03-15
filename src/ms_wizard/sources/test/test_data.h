#ifndef TEST_DATA_H
#define TEST_DATA_H

#include <QObject>
#include <QTest>

class Test_Data : public QObject
{
Q_OBJECT
public:
    explicit Test_Data(QObject *parent = 0);
signals:

private slots:
    void states_data();
    void states();
    void edges_data();
    void edges();
    void model_states_data();
    void model_states();//read states only from statemodel
    void model_states_vs_edges_data();
    void model_states_vs_edges();//read states and relations from statemodel
    void model_controls_data();
    void model_controls();//read states from controlmodel
    void model_inputs_data();
    void model_inputs();//read inputs from inputmodel
    void model_outputs_data();
    void model_outputs();//read outputs from inputmodel
    void editmodel_states_data();
    void editmodel_states();//read states only from statemodel
    void editmodel_inputs_data();
    void editmodel_inputs();//edit  in inputModel
    void editmodel_outputs_data();
    void editmodel_outputs();//edit  in outputModel
    void editmodel_controls_data();
    void editmodel_controls();//edit  in ControlModel
    void dragndrop_outputs_data();
    void dragndrop_outputs();//drag&drop  in outputModel/controlModel
    void remove_data();
    void remove();//drag&drop  in outputModel/controlModel
    void remove_in_models_data();
    void remove_in_models();//drag&drop  in outputModel/controlModel
    void xmlproject_data();
    void xmlproject();//drag&drop  in outputModel/controlModel
    void states_position_data();
    void states_position();//position of states
    void project_common_data();
    void project_common();//common project data
    void gen_verilog_data();
    void gen_verilog();//test for generate verilog
    void gen_log_data();
    void gen_log();//test for generate verilog
    void unique_state_in_out_names_data();
    void unique_state_in_out_names();//test for generate verilog
    void invalid_values_data();
    void invalid_values();//test for generate verilog
};

#endif // TEST_DATA_H
