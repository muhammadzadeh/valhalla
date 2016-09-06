#ifndef __VALHALLA_SKADI_SERVICE_H__
#define __VALHALLA_SKADI_SERVICE_H__

#include <boost/property_tree/ptree.hpp>

#include <valhalla/midgard/pointll.h>
#include <valhalla/skadi/sample.h>

namespace valhalla {
  namespace skadi {

    void run_service(const boost::property_tree::ptree& config);

    class skadi_worker_t {
     public:
      enum ACTION_TYPE {HEIGHT = 0};
      skadi_worker_t(const boost::property_tree::ptree& config);
      virtual ~skadi_worker_t();
      prime_server::worker_t::result_t work(const std::list<zmq::message_t>& job, void* request_info);
      void cleanup();

     protected:

      void init_request(const ACTION_TYPE& action, const boost::property_tree::ptree& request);

      prime_server::worker_t::result_t elevation(const boost::property_tree::ptree& request, prime_server::http_request_t::info_t& request_info);

      std::list<midgard::PointLL> shape;
      boost::optional<std::string> encoded_polyline;
      bool range;
      skadi::sample sample;
      size_t max_shape;
      float min_resample;
      float long_request;
  };
 }
}

#endif //__VALHALLA_SKADI_SERVICE_H__
