#include <gazebo_test/gazebo_ros_quad.h>

namespace gazebo
{

GZ_REGISTER_MODEL_PLUGIN(GazeboRosQuad);

GazeboRosQuad::GazeboRosQuad()
{
  this->wrench_msg_.force.x = 0.0;
  this->wrench_msg_.force.y = 0.0;
  this->wrench_msg_.force.z = 0.0;
  this->wrench_msg_.torque.x = 0.0;
  this->wrench_msg_.torque.y = 0.0;
  this->wrench_msg_.torque.z = 0.0;
}


GazeboRosQuad::~GazeboRosQuad()
{
  event::Events::DisconnectWorldUpdateBegin(this->update_connection_);

  // Custom Callback Queue
  this->queue_.clear();
  this->queue_.disable();
  this->rosnode_->shutdown();
  this->callback_queue_thread_.join();

  delete this->rosnode_;
}


void GazeboRosQuad::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{
  // Get the world name
  this->world_ = _model->GetWorld();
  
  // Load parameters

  // Load robot namespace
  this->robot_namespace_ = ""; // Default value
  if (_sdf->HasElement("robotNamespace"))
    this->robot_namespace_ = _sdf->GetElement("robotNamespace")->Get<std::string>() + "/";
  
  // Load body name
  if (!_sdf->HasElement("bodyName"))
  {
    // Fail to load body name
    ROS_FATAL("gazebo_ros_quad plugin missing <bodyName>, cannot proceed");
    return;
  }
  else
    this->link_name_ = _sdf->GetElement("bodyName")->Get<std::string>();
  // Get link object
  this->link_ = _model->GetLink(this->link_name_);
  if (!this->link_)
  {
    // Fail to get link
    ROS_FATAL("gazebo_ros_quad plugin error: link named: %s does not exist",this->link_name_.c_str());
    return;
  }
  // Load topic name
  this->topic_name_ = "force";
  if (!_sdf->HasElement("topicName"))
  {
    ROS_INFO("gazebo_ros_quad plugin missing <topicName>, using default (topicName = \"force\")");
  }
  else
    this->topic_name_ = _sdf->GetElement("topicName")->Get<std::string>();

  // Make sure the ROS node for Gazebo has already been initialized
  if (!ros::isInitialized())
  {
    ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
      << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
    return;
  }

  this->rosnode_ = new ros::NodeHandle(this->robot_namespace_);

  // Subscriber for custom callback Queue
  ros::SubscribeOptions so = ros::SubscribeOptions::create<geometry_msgs::Wrench>(
    this->topic_name_, // Topic name
    1, // Queue size
    boost::bind( &GazeboRosQuad::UpdateObjectForce,this,_1), // Callback
    ros::VoidPtr(), // Tracked object
    &this->queue_); // Queue
  this->sub_ = this->rosnode_->subscribe(so);

  // Custom Callback Queue
  this->callback_queue_thread_ = boost::thread( boost::bind( &GazeboRosQuad::QueueThread,this ) );

  // New Mechanism for Updating every World Cycle
  // Listen to the update event. This event is broadcast every simulation iteration.
  this->update_connection_ = event::Events::ConnectWorldUpdateBegin(
      boost::bind(&GazeboRosQuad::UpdateChild, this));
}

// Update the controller
void GazeboRosQuad::UpdateObjectForce(const geometry_msgs::Wrench::ConstPtr& _msg)
{
  this->wrench_msg_.force.x = _msg->force.x;
  this->wrench_msg_.force.y = _msg->force.y;
  this->wrench_msg_.force.z = _msg->force.z;
  this->wrench_msg_.torque.x = _msg->torque.x;
  this->wrench_msg_.torque.y = _msg->torque.y;
  this->wrench_msg_.torque.z = _msg->torque.z;
}

// Update the controller
void GazeboRosQuad::UpdateChild()
{
  {
    boost::mutex::scoped_lock lock(this->lock_);
    math::Vector3 force(this->wrench_msg_.force.x,this->wrench_msg_.force.y,this->wrench_msg_.force.z);
    math::Vector3 torque(this->wrench_msg_.torque.x,this->wrench_msg_.torque.y,this->wrench_msg_.torque.z);
    this->link_->AddForce(force);
    this->link_->AddTorque(torque);
  }
}

// Custom Callback Queue
void GazeboRosQuad::QueueThread()
{
  static const double timeout = 0.01;

  while (this->rosnode_->ok())
  {
    this->queue_.callAvailable(ros::WallDuration(timeout));
  }
}

}
