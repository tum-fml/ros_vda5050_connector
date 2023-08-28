#ifndef ORDER_H
#define ORDER_H

#include "vda5050_msgs/Order.h"

/**
 * @brief Wrapper class to add functionalities to the VDA 5050 Order messages.
 *
 */
class Order {
 public:
  /**
   * @brief Construct a new Order object.
   *
   */
  Order();

  /**
   * @brief Construct a new Order object from a vda5050 ros message.
   *
   * @param order
   */
  Order(const vda5050_msgs::Order::ConstPtr& order);

  /**
   * @brief Accepts an order by clearing all old nodes and edges, and sets the nodes, edges, orderId
   * and orderUpdateId.
   *
   * @param new_order
   */
  void AcceptNewOrder(const Order& new_order);

  /**
   * @brief Updates the current order with the new nodes, edges and action received in the order
   * update.
   *
   * Sets the order update id.
   *
   * @param order_update
   */
  void UpdateOrder(const Order& order_update);

  /**
   * @brief Checks if the order is valid by testing the number of nodes, edges, and validating the
   * order sequence.
   *
   */
  void Validate();

  // ----- Getters and Setters -----

  /**
   * @brief Get the Order Id.
   *
   * @return std::string
   */
  inline std::string GetOrderId() const { return order.orderId; };

  /**
   * @brief Get the Order Update Id.
   *
   * @return uint32_t
   */
  inline uint32_t GetOrderUpdateId() const { return order.orderUpdateId; };

  inline std::string GetZoneSetId() const { return order.zoneSetId; }
  /**
   * @brief Get all the nodes in the order
   *
   * @return std::vector<vda5050_msgs::Node>
   */
  inline const std::vector<vda5050_msgs::Node>& GetNodes() const { return order.nodes; }

  /**
   * @brief Get all the edges in the order.
   *
   * @return std::vector<vda5050_msgs::Edge>
   */
  inline const std::vector<vda5050_msgs::Edge>& GetEdges() const { return order.edges; }

 private:
  vda5050_msgs::Order order; /**< Order message */
};

#endif