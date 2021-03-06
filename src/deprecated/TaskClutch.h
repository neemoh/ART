
// Created by Andre Eddy on 06/08/2017.
//

#ifndef ATAR_TASKCLUTCH_H
#define ATAR_TASKCLUTCH_H


#include "src/ar_core/SimTask.h"

#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkLineSource.h>
#include <vtkImageActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkTransform.h>
#include <vtkSTLReader.h>
#include <vtkProperty.h>
#include <vtkParametricTorus.h>
#include <vtkParametricFunctionSource.h>
#include <vtkCellLocator.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkPolyDataNormals.h>
#include <vtkCornerAnnotation.h>

#include "src/ar_core/Rendering.h"
#include "custom_msgs/ActiveConstraintParameters.h"
#include "custom_msgs/TaskState.h"
#include "src/ar_core/BulletVTKMotionState.h"
#include <ros/ros.h>
#include <std_msgs/Empty.h>

#include <btBulletDynamicsCommon.h>
#include "src/ar_core/SimObject.h"
#include <vtkMinimalStandardRandomSequence.h>



class TaskClutch : public SimTask{
public:

    TaskClutch(const std::string mesh_files_dir,
                   const bool show_ref_frames, const bool num_tools,
                   const bool with_guidance);

    ~TaskClutch();

    // returns all the task graphics_actors to be sent to the rendering part
    std::vector< vtkSmartPointer <vtkProp> > GetActors() {
        return graphics_actors;
    }

    // updates the task logic and the graphics_actors
    void StepWorld();

    custom_msgs::TaskState GetTaskStateMsg();

    // check if the task is finished
    void TaskEvaluation();

    // check if the pointer is in the target place
    void PoseEvaluation();

    // resets the number of repetitions and task state;
    void ResetTask();


    // decrements the number of repetitions. Used in case something goes
    // wrong during that repetition.
    void ResetCurrentAcquisition();


    /**
    * \brief This is the function that is handled by the desired pose thread.
     * It first reads the current poses of the tools and then finds the
     * desired pose from the mesh.
  *  **/
    void HapticsThread();

    void InitBullet();

    void StepPhysics();




private:
    std::vector<std::array<double, 3> > sphere_positions;


    custom_msgs::TaskState task_state_msg;
    enum class TaskState: uint8_t {Idle, Reaching};
    TaskState task_state;


    std::vector<double> kine_pointer_dim;
    double board_dimensions[3];
    double peg_dimensions[3];
    double sides;
    bool out[4];
    double actual_distance;
    double target_distance;
    ros::Time start_pause;
    SimObject* kine_p;
    bool count = 0;
    double* peg_pose1;
    double* peg_pose2;
    double* peg_pose3;
    double* peg_pose4;
    float height;
    BulletVTKMotionState* motion_state_;
    std::vector<double> target_pos;
    KDL::Vector previous_point;
    btDiscreteDynamicsWorld* dynamicsWorld;
    ros::Time time_last;
    float threshold=0.5;
    double color[3];
    int box_n;
    bool start=0;
    uint8_t rep = 1;
    double ACP;
    KDL::Rotation rot;
    KDL::Rotation rot_inv;
    KDL::Vector box_pose;

    // Timing
    double time;
    ros::Time begin;

    bool cond=0;
    int dt=0;

    int init=1;
    int task_rep=0;
    int num_task_max=6;


    // Definition of chessboard number of cols and rows
    int cols = 6;
    int rows = 6;
    // NB You have to set manually the dim of chessboard since it has to be a
    // constant (it is simply cols*rows)

    SimObject* chessboard[36];

    double* ideal_position;
    KDL::Vector pointer_posit;
    KDL::Vector distance;

    // -------------------------------------------------------------------------
    // graphics

    // for not we use the same type of active constraint for both arms
    custom_msgs::ActiveConstraintParameters ac_parameters;

    KDL::Frame tool_desired_pose_kdl[2];
    KDL::Frame *tool_current_pose_kdl[2];
    double *gripper_position[2];
//    vtkSmartPointer<vtkActor>                       d_board_actor;
//    std::vector< vtkSmartPointer<vtkActor>>         d_sphere_actors;

    int peg_type=1; // 1 = spheres, 0= cubes


};

#endif //ATAR_TASKBULLETt_H

