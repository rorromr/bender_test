/*
* ROS Quad Gazebo plugin
* 
*/

#ifndef GAZEBO_ROS_QUAD_HH
#define GAZEBO_ROS_QUAD_HH

#include <string>

// Custom Callback Queue
#include <ros/callback_queue.h>
#include <ros/subscribe_options.h>
#include <geometry_msgs/Wrench.h>

#include <ros/ros.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <gazebo/physics/physics.hh>
#include <gazebo/transport/TransportTypes.hh>
#include <gazebo/common/Plugin.hh>
#include <gazebo/common/Events.hh>

/*
* Usage
* <gazebo>
*   <plugin filename="libgazebo_ros_quad.so" name="gazebo_ros_quad">
*     <bodyName>box_body</bodyName>
*     <topicName>box_force</topicName>
*   </plugin>
* </gazebo>
*
*/

namespace gazebo
{

class GazeboRosQuad : public ModelPlugin
{
  
  public: GazeboRosQuad();

  public: virtual ~GazeboRosQuad();

  // Inherited
  protected: void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

  // Inherited
  protected: virtual void UpdateChild();

  /**
   * @brief Callback function when a wrench message is published in ROS topic
   * @param _msg ROS wrench message
   */
  private: void UpdateObjectForce(const geometry_msgs::Wrench::ConstPtr& _msg);

  /**
   * @brief Custom callback queue thread function
   */
  private: void QueueThread();

  // Pointer to the gazebo world
  private: physics::WorldPtr world_;

  // Pointer to the Link, where force is applied
  private: physics::LinkPtr link_;

  // Pointer to the ROS node.  A node will be instantiated if it does not exist.
  private: ros::NodeHandle* rosnode_;
  private: ros::Subscriber sub_;

  // Mutex to lock access to fields that are used in ROS message callbacks
  private: boost::mutex lock_;

  // ROS Wrench topic name
  private: std::string topic_name_;
  // The Link this plugin is attached to, and will exert forces on.
  private: std::string link_name_;

  // ROS namespace
  private: std::string robot_namespace_;

  // Custom Callback Queue
  private: ros::CallbackQueue queue_;
  // Thead for the running callback thread
  private: boost::thread callback_queue_thread_;
  // Container for the wrench force that this plugin exerts on the body.
  private: geometry_msgs::Wrench wrench_msg_;

  // Pointer to the update event connection
  private: event::ConnectionPtr update_connection_;
};

}
//GAZEBO_ROS_QUAD_HH
#endif
