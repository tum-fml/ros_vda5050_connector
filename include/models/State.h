#ifndef STATE_H
#define STATE_H

#include <ros/ros.h>

#include <boost/optional.hpp>

#include "models/Order.h"
#include "vda5050_msgs/Connection.h"
#include "vda5050_msgs/Factsheet.h"
#include "vda5050_msgs/InstantAction.h"
#include "vda5050_msgs/InteractionZoneStates.h"
#include "vda5050_msgs/Node.h"
#include "vda5050_msgs/State.h"
#include "vda5050_msgs/Visualization.h"

/**
 * @brief Wrapper class to add functionalities to the VDA 5050 State messages.
 *
 */
class State {
 public:
  /**
   * @brief Default constructer for the State.
   *
   */
  State();

  /**
   * @brief Accepts an order by clearing all state arrays, setting the nodeStates, edgeStates and
   * actionStates.
   *
   * The orderId and orderUpdateId are also updated.
   *
   * @param new_order
   */
  void AcceptNewOrder(const Order& new_order);

  /**
   * @brief Accepts action states from pre-filled list.
   *
   */
  void AddInstantActionStates(vda5050_msgs::InstantAction& action);

  /**
   * @brief Checks if the state has an order that is currently being executed.
   *
   * @return true
   * @return false
   */
  bool HasActiveOrder(const Order& current_order);

  /**
   * @brief Checks if the order update correctly continues on the previous order by comparing the
   * last node in the base against the received update.
   *
   * @param order_update
   */
  void ValidateUpdateBase(const Order& order_update);

  /**
   * @brief Adds the new nodes, edges and actions from an order update to the state message.
   *
   * Clears horizon.
   *
   * @param order_update
   */
  void UpdateOrder(const Order& current_order, const Order& order_update);

  /**
   * @brief Searches in the current state for the last released node in the order.
   *
   * @return Optional value that contains the last node in the base if it was found.
   */
  boost::optional<vda5050_msgs::NodeState> GetLastNodeInBase();

  /**
   * @brief Appends the provided error to the list of errors in the state message. If an error type
   * already exists, it is replaced with the provided error.
   *
   * @param error
   */
  void AppendError(const vda5050_msgs::Error& error);

  /**
   * @brief Removes errors from the errors list based on their error type.
   *
   * @param error
   */
  void ClearErrorWithType(const std::string& error_type);

  /**
   * @brief Removes all errors from the state message.
   *
   * @param error
   */
  inline void ClearAllErrors() { state.errors.clear(); };

  /**
   * @brief Create a Visualization message from the current state message.
   *
   * @return vda5050_msgs::Visualization
   */
  vda5050_msgs::Visualization CreateVisualizationMsg();

  /**
   * @brief Create a Connection message from the current state message.
   *
   * @return vda5050_msgs::Connection
   */
  vda5050_msgs::Connection CreateConnectionMsg();

  /**
   * @brief Tests if the robot's position is within the deviation range of the provided node.
   *
   * @return true
   * @return false
   */
  bool InDeviationRange(vda5050_msgs::Node);

  // ----- Getters and Setters -----

  /**
   * @brief Get the State object
   *
   * @return vda5050_msgs::State
   */
  inline vda5050_msgs::State GetState() { return state; }

  /**
   * @brief Get the Factsheet object
   *
   * @return vda5050_msgs::Factsheet
   */
  inline vda5050_msgs::Factsheet GetFactsheet() { return factsheet; }

  // Header information.

  /**
   * @brief Set the Header Id in the message header.
   *
   * @param header_id
   */
  inline void SetHeaderId(const int header_id) { state.headerId = header_id; }

  /**
   * @brief Set the Timestamp in the message header.
   *
   * @param timestamp
   */
  inline void SetTimestamp(const std::string& timestamp) { state.timestamp = timestamp; }

  /**
   * @brief Set the Manufacturer field in the message header.
   *
   * @param manufacturer
   */
  inline void SetManufacturer(const std::string& manufacturer) {
    state.manufacturer = manufacturer;
  }

  /**
   * @brief Get the Manufacturer from the state message.
   *
   * @return std::string
   */
  inline std::string GetManufacturer() { return state.manufacturer; }

  /**
   * @brief Set the Version in the message header.
   *
   * @param version
   */
  inline void SetVersion(const std::string& version) { state.version = version; }

  /**
   * @brief Get the version from the state message.
   *
   * @return std::string
   */
  inline std::string GetVersion() { return state.version; }

  /**
   * @brief Set the Serial Number in the message header.
   *
   * @param sn
   */
  inline void SetSerialNumber(const std::string& sn) { state.serialNumber = sn; }

  /**
   * @brief Get the Serial Number from the state message.
   *
   * @return std::string
   */
  inline std::string GetSerialNumber() { return state.serialNumber; }

  /**
   * @brief Set the id of the zone set being used when navigating.
   *
   * @param zone_set_id
   */
  inline void SetZoneSetId(const std::string& zone_set_id) { state.zoneSetId = zone_set_id; }

  // Order related information.

  /**
   * @brief Get the id of the running order. On startup, this will always return an empty string.
   *
   * @return std::string
   */
  inline const std::string& GetOrderId() { return state.orderId; };

  /**
   * @brief Return the order update id. 0 is the default value.
   *
   * @return uint32_t
   */
  inline const uint32_t GetOrderUpdateId() { return state.orderUpdateId; };

  // Battery information.

  /**
   * @brief Set the battery charge (Percentage)
   *
   * @param battery_charge
   * @return true if the value is between 0.0 and 100.0
   */
  inline bool SetBatteryCharge(const double battery_charge) {
    if (0.0 <= battery_charge <= 100.0) {
      state.batteryState.batteryCharge = battery_charge;
      return true;
    };
    return false;
  }

  /**
   * @brief Set the battery voltage.
   *
   * @param battery_voltage
   */
  inline void SetBatteryVoltage(const double battery_voltage) {
    state.batteryState.batteryVoltage = battery_voltage;
  }

  /**
   * @brief Set the charging field in the state message.
   *
   * @param charging
   */
  inline void SetBatteryCharging(const bool charging) { state.batteryState.charging = charging; }

  // Other State message fields.

  /**
   * @brief Set the Operating Mode object
   *
   * @param operating_mode
   * @return true
   * @return false
   */
  inline bool SetOperatingMode(const std::string& operating_mode) {
    if (operating_mode != vda5050_msgs::State::AUTOMATIC &&
        operating_mode != vda5050_msgs::State::SEMIAUTOMATIC &&
        operating_mode != vda5050_msgs::State::MANUAL &&
        operating_mode != vda5050_msgs::State::SERVICE &&
        operating_mode != vda5050_msgs::State::TEACHIN) {
      return false;
    }
    state.operatingMode = operating_mode;
    return true;
  }

  /**
   * @brief Set the errors array.
   *
   * @param errors
   */
  inline void SetErrors(const std::vector<vda5050_msgs::Error> errors) { state.errors = errors; }

  /**
   * @brief Set the information array.
   *
   * @param information
   */
  inline void SetInformation(const std::vector<vda5050_msgs::Info> information) {
    state.information = information;
  }

  /**
   * @brief Get the driving field.
   *
   * @return true
   * @return false
   */
  inline bool GetDriving() { return state.driving; }

  /**
   * @brief Set the driving field.
   *
   * @param driving
   */
  inline void SetDriving(const bool driving) { state.driving = driving; }

  // AGV Position field.

  /**
   * @brief Set the position of the vehicle on the map.
   *
   * @param x
   * @param y
   * @param theta
   */
  inline void SetAGVPosition(const double x, const double y, const double theta) {
    state.agvPosition.x = x;
    state.agvPosition.y = y;
    state.agvPosition.theta = theta;
  }

  /**
   * @brief Set the localization score.
   *
   * @param score
   * @return true if the score is updated
   * @return false if the provided score is outisde the range of [0.0, 1.0]
   */
  inline bool SetLocalizationScore(const double score) {
    if (0.0 <= score <= 1.0) {
      state.agvPosition.localizationScore = score;
      return true;
    }
    return false;
  }

  /**
   * @brief Set the position initialized of the vehicle.
   *
   * @param initialized
   */
  inline void SetPositionInitialized(const bool initialized) {
    state.agvPosition.positionInitialized = initialized;
  }

  /**
   * @brief Set the id of the map being used by the vehicle.
   *
   * @param map_id
   */
  inline void SetMapId(const std::string& map_id) { state.agvPosition.mapId = map_id; }

  /**
   * @brief Set the vehicle's velocity.
   *
   * @param vel
   */
  inline void SetVelocity(const vda5050_msgs::Velocity vel) { state.velocity = vel; }

  /**
   * @brief Set the vehicle's loads.
   *
   * @param loads
   */
  inline void SetLoads(const std::vector<vda5050_msgs::Load> loads) { state.loads = loads; }

  /**
   * @brief Set the value of the paused field.
   *
   * @param paused
   */
  inline void SetPaused(const bool paused) { state.paused = paused; }

  /**
   * @brief Set the value of a new base request. When the vehicle is executing an order, and wants
   * to request an update because it is almost done with the actions, the value needs to be set to
   * true.
   *
   * @param new_base_req
   */
  inline void SetNewBaseRequest(const bool new_base_req) { state.newBaseRequest = new_base_req; }

  /**
   * @brief Set the distance traveresed by the robot since the last traveresed node.
   *
   * @param distance
   */
  inline void SetDistanceSinceLastNode(const double distance) {
    state.distanceSinceLastNode = distance;
  }

  /**
   * @brief Set the safety state of the vehicle.
   *
   * @param safety_state
   */
  inline void SetSafetyState(const vda5050_msgs::SafetyState& safety_state) {
    state.safetyState = safety_state;
  }

  /**
   * @brief Set the safety state of the vehicle.
   *
   * If the provided zoneStatus is different than 0, then it's forced to 1 to be understood by MC.
   *
   * @param safety_state
   */
  inline void SetInteractionZones(const vda5050_msgs::InteractionZoneStates& interaction_zones) {
    auto zones = interaction_zones;

    for (auto& zone : zones.interactionZones) {
      if (zone.zoneStatus != 0) zone.zoneStatus = 1;
    }

    // state.interactionZones = zones.interactionZones;
  }

  /**
   * @brief Fill the state from a pre-filled state message containing information about the running
   * order.
   *
   */
  inline void SetOrderState(const vda5050_msgs::State order_state) {
    // Check orderId and orderUpdateId
    if (state.orderId != order_state.orderId)
      ROS_ERROR_STREAM("Received order state for id " << order_state.orderId
                                                      << " but current order is " << state.orderId);
    else {
      if (!order_state.nodeStates.empty()) {
        // If last node has changed: remove the old ones
        if (order_state.lastNodeSequenceId != state.lastNodeSequenceId ||
            state.nodeStates.begin()->sequenceId != order_state.nodeStates.begin()->sequenceId) {
          if (state.nodeStates.begin()->sequenceId == order_state.lastNodeSequenceId) {
            state.nodeStates.erase(state.nodeStates.begin());
            state.lastNodeId = order_state.lastNodeId;
            state.lastNodeSequenceId = order_state.lastNodeSequenceId;
          } else {
            auto it = find_if(state.nodeStates.begin(), state.nodeStates.end(),
                [&](const vda5050_msgs::NodeState& ns) {
                  return ns.sequenceId == state.lastNodeSequenceId;
                });
            if (it != state.nodeStates.end()) {
              state.nodeStates.erase(state.nodeStates.begin(), it);
              state.lastNodeId = order_state.lastNodeId;
              state.lastNodeSequenceId = order_state.lastNodeSequenceId;
            } else
              ROS_ERROR_STREAM("ERROR: last node not found");
          }
        }
      } else {
        state.lastNodeId = order_state.lastNodeId;
        state.lastNodeSequenceId = order_state.lastNodeSequenceId;
        state.nodeStates.clear();
      }

      // Edges
      if (!state.edgeStates.empty()) {
        if (!order_state.edgeStates.empty()) {
          if (state.edgeStates.begin()->sequenceId != order_state.edgeStates.begin()->sequenceId) {
            auto it = find_if(state.edgeStates.begin(), state.edgeStates.end(),
                [&](const vda5050_msgs::EdgeState& es) {
                  return es.sequenceId == order_state.edgeStates.begin()->sequenceId;
                });
            if (it != state.edgeStates.end()) {
              state.edgeStates.erase(state.edgeStates.begin(), it);
            } else
              ROS_ERROR_STREAM("ERROR: last edge not found");
          }
        } else {
          state.edgeStates.clear();
        }
      }
      for (int a = 0; a < order_state.actionStates.size(); a++)
        SetActionState(order_state.actionStates[a]);
    }
  }

  /**
   * @brief Fill action states from pre-filled list.
   *
   */
  inline void SetActionState(const vda5050_msgs::ActionState updated_as) {
    auto it = find_if(state.actionStates.begin(), state.actionStates.end(),
        [&](const vda5050_msgs::ActionState& as) { return updated_as.actionId == as.actionId; });

    if (it != state.actionStates.end()) {
      it->actionStatus = updated_as.actionStatus;
      it->resultDescription = updated_as.resultDescription;
    } else
      ROS_ERROR_STREAM(
          "Action state with id " << updated_as.actionId << " not found in state message");
  }

  /**
   * @brief Fill action states from pre-filled list.
   *
   */
  inline void SetActionState(
      std::string action_id, std::string action_status, std::string result_description = "") {
    auto it = find_if(state.actionStates.begin(), state.actionStates.end(),
        [&](const vda5050_msgs::ActionState& as) { return action_id == as.actionId; });

    if (it != state.actionStates.end()) {
      it->actionStatus = action_status;
      it->resultDescription = result_description;
    } else
      ROS_ERROR_STREAM("Action state with id " << action_id << " not found in state message");
  }

  /**
   * @brief Set factsheet message.
   */
  inline void SetFactsheet(const vda5050_msgs::Factsheet factsheet_msg) {
    factsheet = factsheet_msg;
    factsheet.version = state.version;
    factsheet.manufacturer = state.manufacturer;
    factsheet.serialNumber = state.serialNumber;
  }

 private:
  vda5050_msgs::State state; /**< State message */

  vda5050_msgs::Factsheet factsheet; /**< Factsheet message */

  /**
   * @brief Transform a VDA Node to a Node state object.
   *
   * @param n
   * @return vda5050_msgs::NodeState
   */
  vda5050_msgs::NodeState NodeToNodeState(const vda5050_msgs::Node& n);

  /**
   * @brief Transform a VDA Edge to an Edge state object.
   *
   * @param e
   * @return vda5050_msgs::EdgeState
   */
  vda5050_msgs::EdgeState EdgeToEdgeState(const vda5050_msgs::Edge& e);

  /**
   * @brief Transform a VDA Action to an Action state object.
   *
   * @param a
   * @return vda5050_msgs::ActionState
   */
  vda5050_msgs::ActionState ActionToActionState(const vda5050_msgs::Action& a);
};

#endif