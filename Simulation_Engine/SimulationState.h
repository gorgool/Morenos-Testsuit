#pragma once

#include <cinttypes>
#include <stdexcept>
#include <iterator>
#include <array>

#include "SearchResult_Msg.h"

/**
 * @class TargetState
 * 
 * @brief Describes a single target state and provide interface for setting up parameters and advance state.  
 * 
 */
class TargetState
{
public:
    TargetState();

    /**
   * @fn TargetState& TargetState::set_coordinates(double u, double v, double u_vel, double v_vel);
   *
   * @brief Sets coordinates for target object.
   *
   * @param u Initaial u coordinate in cosine space.
   * @param v Initiial v coordiante in cosine space.
   * @param u_vel U coordiante rate of change in a second.
   * @param v_vel V coordiante rate of change in a second.
   * 
   * @exception std::invalid_argument Thrown if coordinates out of valid range.
   * 
   * @return Referance to this object with updated parameters.
   */
    TargetState& set_coordinates(double u, double v, double u_vel, double v_vel);

    /**
   * @fn TargetState& TargetState::set_channel(std::uint8_t id);
   *
   * @brief Sets a channel for target object.
   *
   * @param id Id of the channel.
   * 
   * @exception std::invalid_argument Thrown if channel out of valid range.
   * 
   * @return Referance to this object with updated parameters.
   */
    TargetState& set_channel(std::uint8_t id);

    /**
   * @fn TargetState& TargetState::set_variance(double variance);
   *
   * @brief Sets variance of coordinates.
   *
   * @param variance value of the variance, dimensionless (cosine space).
   * 
   * @exception std::invalid_argument Thrown if variance out of valid range.
   * 
   * @return Referance to this object with updated parameters.
   */
    TargetState& set_variance(double variance);

    /**
   * @fn TargetState& TargetState::set_power(std::uint16_t power);
   *
   * @brief Sets the reflected "power" of the target.
   *
   * @param power Amount of power to be reflected, dimensionless.
   * 
   * @return Referance to this object with updated parameters.
   */
    TargetState& set_power(std::uint16_t power);

    /**
   * @fn TargetState& TargetState::set_frequency(double start, double width);
   *
   * @brief Sets targets operating frequency range.
   *
   * @param start Start of the frequency range, MHz.
   * @param width Width of the frequncy range, MHz.
   * 
   * @exception std::invalid_argument Thrown if frequency out of valid range.
   * 
   * @return Referance to this object with updated parameters.
   */
    TargetState& set_frequency(double start, double width);

    /**
   * @fn TargetState& TargetState::advance(std::uint64_t time);
   *
   * @brief Update internal target state up to given time. If delta time (between 
   *        given time and previos time) is larger than some threshold state not updated
   *        and given time become previos time. This allows to do initial time binding. 
   *
   * @param time Time on which internal target state should be computed, milliseconds.
   */
	void advance(std::uint64_t time);

    /**
   * @fn TargetState& TargetState::reset();
   *
   * @brief Sets internal state in the initial state.
   *
   */
    void reset();

    /**
   * @fn TargetState& TargetState::get_message() const;
   *
   * @brief Convert internal target state into PlotDescription.
   * 
   * @return PlotDescription struct.
   */
	PlotDescription get_message() const;

    /**
   * @fn TargetState& TargetState::get_u() const;
   *
   * @brief Return current u coordinate.
   * 
   * @return Current u coordinate.
   */
    double get_u() const { return current_u_; }

   /**
   * @fn TargetState& TargetState::get_v() const;
   *
   * @brief Return current v coordinate.
   * 
   * @return Current v coordinate.
   */
    double get_v() const { return current_v_; }

private:

    /** @brief Time threshold for time rebinding, milliseconds*/
    const std::uint64_t delta_threshold_ = 10000;

    /** @brief Time at which the last advance function have been called, milliseconds. */
    std::uint64_t prev_timestamp_;

    /** @brief Initial u coordinate of target in cosine space. */
    double initial_u_;

    /** @brief Initial v coordinate of target in cosine space. */
    double initial_v_;

    /** @brief Current u coordinate of target in cosine space. */
    double current_u_;

    /** @brief Current v coordinate of target in cosine space. */
    double current_v_;

    /** @brief Velocity of the target in u coordinate, rate in a second */
    double u_vel_;

    /** @brief Velocity of the target in v coordinate, rate in a second */
    double v_vel_;

    /** @brief Channel ID. */
    std::uint8_t channel_id_;

    /** @brief Variance of the coordinates. */
    double variance_;

    /** @brief Reflected power. */
    std::uint16_t power_;

    /** @brief Start of the frequency range, MHz. */
    double freq_range_start_;

    /** @brief Width of the frequency range, MHz. */
    double freq_range_width_;
};

/**
 * @class EnvironmentState
 * 
 * @brief Describes an environment state (that shared with all models) and a set of targets. Additionaly provides
 *        interface for setting up parameters and advancing environment state and state of all targets. 
 * 
 */
class EnvironmentState
{
public:

    EnvironmentState();

    /**
   * @fn TargetState& TargetState::set_polarization(std::uint8_t polarization);
   *
   * @brief Sets polarization type for all targets.
   *
   * @param polarization Type of polarization.
   * 
   * @exception std::invalid_argument Thrown if polarization out of valid range.
   * 
   * @return Referance to this object with updated parameters.
   */
    EnvironmentState& set_polarization(std::uint8_t polarization);

    /**
   * @fn TargetState& TargetState::set_search_id(std::uint8_t id);
   *
   * @brief Sets ID of the search area.
   *
   * @param id ID of the search area.
   * 
   * @return Referance to this object with updated parameters.
   */
    EnvironmentState& set_search_id(std::uint8_t id);

    /**
   * @fn TargetState& TargetState::set_gain(std::array<double, 8> gain);
   *
   * @brief Sets gain coeffecients for all 8 channels.
   *
   * @param gain Array of the gain coefficients in DB.
   * 
   * @exception std::invalid_argument Thrown if gain coefficients out of valid range.
   * 
   * @return Referance to this object with updated parameters.
   */
    EnvironmentState& set_gain(std::array<double, 8> gain);

    /**
   * @fn TargetState& TargetState::set_target(TargetState&& target);
   *
   * @brief Adds (moves) target to list.
   *
   * @param target Target to be added to list.
   * 
   * @return Referance to this object with updated parameters.
   */
    EnvironmentState& set_target(TargetState&& target);

   /**
   * @fn TargetState& TargetState::set_target(TargetState&& target);
   *
   * @brief Adds (copy) target to list.
   *
   * @param target Target to be added to list.
   * 
   * @return Referance to this object with updated parameters.
   */
    EnvironmentState& set_target(const TargetState& target);

    /**
   * @fn TargetState& TargetState::set_uv_limits(const double u_left, const double u_right, const double v_left, const double v_right);
   *
   * @brief Sets working area limits in UV cosinespace.
   *
   * @param u_left Left border of working area in cosine space..
   * @param u_right Right border of working area in cosine space..
   * @param v_left Upper border of working area in cosine space.
   * @param v_right Buttom border of working area in cosine space..
   * 
   * @exception std::invalid_argument Thrown if values out of valid range.
   * 
   * @return Referance to this object with updated parameters.
   */
    EnvironmentState& set_uv_limits(const double u_left, const double u_right, const double v_left, const double v_right);

    /**
   * @fn TargetState& TargetState::check_limits(const double u, const double v) const;
   *
   * @brief Check if given coordinates are within working area.
   *
   * @param u U coordinate in cosine space.
   * @param v V coordinate in cosine space.
   * 
   * @return Referance to this object with updated parameters.
   */
    bool check_limits(const double u, const double v) const;

    /**
   * @fn TargetState& TargetState::advance(std::uint64_t time);
   *
   * @brief Update internal state and state of all targets up to given time. If resulting 
   * oordinates of particular target are not within working area reset its state. 
   *
   * @param time Time on which internal target state should be computed, milliseconds.
   */
	void advance(std::uint64_t time);

    /**
   * @fn TargetState& TargetState::get_message() const;
   *
   * @brief Convert internal environment state into SearchResult (27.1) message.
   * 
   * @return SearchResult message.
   */
	SearchResult_Msg get_message() const;

private:

    /** @brief Time at which the last advance function have been called, milliseconds. */
    std::uint64_t timestamp_;

    /** @brief List of all targets. */
    std::vector<TargetState> targets_;

    /** @brief Polarization type. */
    std::uint8_t polarization_type_;

    /** @brief Gian coefficients in all 8 channels, DB. */
    double signal_amp_[8];

    /** @brief ID of search area in wich all targets are belong. */
    std::uint8_t search_area_id_;

    /** @brief Left border of working area in cosine space. */
    double u_limit_left_;

    /** @brief Right border of working area in cosine space. */
    double u_limit_right_;

    /** @brief Upper border of working area in cosine space. */
    double v_limit_left_;

    /** @brief Buttom border of working area in cosine space. */
    double v_limit_right_;
};
