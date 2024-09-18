#ifndef DEPTH_SEGMENTATION_ROS2_COMMON_H_
#define DEPTH_SEGMENTATION_ROS2_COMMON_H_
//here we define the topics and frames that we will use in the code
// DEPTH_SEGMENTATION_ROS_COMMON_H_ is a macro that is used to avoid multiple inclusion of the same file
#include <string>

#include <rclcpp/rclcpp.hpp>

namespace depth_segmentation {
const static std::string kRgbImageTopic = "/camera/rgb/image_raw";
const static std::string kRgbCameraInfoTopic = "/camera/rgb/camera_info";
const static std::string kDepthImageTopic =
    "/camera/depth_registered/image_raw";
const static std::string kDepthCameraInfoTopic =
    "/camera/depth_registered/camera_info";
const static std::string kSemanticInstanceSegmentationTopic =
    "/mask_rcnn/result";

const static std::string kTfWorldFrame = "map";
const static std::string kTfDepthCameraFrame = "camera0";

}  // namespace depth_segmentation

#endif  // DEPTH_SEGMENTATION_ROS2_COMMON_H_
