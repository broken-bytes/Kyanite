#pragma once

#include <algorithm>
#include <stdint.h>
#include <vector>

namespace input::dualsense {
    /// <remarks>
    /// Actual effect uint8_t values sent to the controller. More complex effects may be build through the combination of these
    /// values and specific paramaters.
    /// </remarks>
    enum class TriggerEffectType : uint8_t {
        // Offically recognized modes
        // These are 100% safe and are the only effects that modify the trigger status nybble
        Off       = 0x05, // 00 00 0 101
        Feedback  = 0x21, // 00 10 0 001
        Weapon    = 0x25, // 00 10 0 101
        Vibration = 0x26, // 00 10 0 110

        // Unofficial but unique effects left in the firmware
        // These might be removed in the future
        Bow       = 0x22, // 00 10 0 010
        Galloping = 0x23, // 00 10 0 011
        Machine   = 0x27, // 00 10 0 111

        // Leftover versions of offical modes with simpler logic and no paramater protections
        // These should not be used
        Simple_Feedback  = 0x01, // 00 00 0 001
        Simple_Weapon    = 0x02, // 00 00 0 010
        Simple_Vibration = 0x06, // 00 00 0 110

        // Leftover versions of offical modes with limited paramater ranges
        // These should not be used
        Limited_Feedback = 0x11, // 00 01 0 001
        Limited_Weapon   = 0x12, // 00 01 0 010

        // Debug or Calibration functions
        // Don't use these as they will courrupt the trigger state until the reset button is pressed
        DebugFC = 0xFC, // 11 11 1 100
        DebugFD = 0xFD, // 11 11 1 101
        DebugFE = 0xFE, // 11 11 1 110
    };

    /**
     * Changelog
     * Revision 1: Initial
     * Revision 2: Added Apple approximated adapter factories. (This may not be correct, please test if you have access to Apple APIs.)
     *             Added Sony factories that use Sony's names.
     *             Added Raw factories for Resistance and AutomaticGun that give direct access to bit-packed region data.
     *             Added ReWASD factories that replicate reWASD effects, warts and all.
     *             Trigger enumerations now and wrapper classes static.
     *             Minor documentation fixes.
     * Revision 3: Corrected Apple factories based on new capture log tests that show only simple rounding was needed.
     * Revision 4: Added 3 new Apple factories based on documentation and capture logs.
     *             These effects are not fully confirmed and are poorly documented even in Apple's docs.
     *             Two of these new effects are similar to our existing raw effect functions.
     * Revision 5: Reorganized and renamed functions and paramaters to be more inline with Sony's API.
     *             Information on the API was exposed by Apple and now further Steamworks version 1.55.
     *             Information is offically source from Apple documentation and Steamworks via logging
     *             HID writes to device based in inputs to new Steamworks functions. Interestingly, my
     *             Raw factories now have equivilents in Sony's offical API and will also be renamed.
     *             Full change list:
     *               TriggerEffectType Enum is re-organized for clarity and comment typoes corrected
     *               TriggerEffectType::Reset is now TriggerEffectType::Off
     *               TriggerEffectType::Resistance is now TriggerEffectType::Feedback
     *               TriggerEffectType::SemiAutomaticGun is now TriggerEffectType::Weapon
     *               TriggerEffectType::AutomaticGun is now TriggerEffectType::Vibration
     *               TriggerEffectType::SimpleResistance is now TriggerEffectType::Simple_Feedback
     *               TriggerEffectType::SimpleSemiAutomaticGun is now TriggerEffectType::Simple_Weapon
     *               TriggerEffectType::SimpleAutomaticGun is now TriggerEffectType::Simple_Vibration
     *               TriggerEffectType::LimitedResistance is now TriggerEffectType::Limited_Feedback
     *               TriggerEffectType::LimitedSemiAutomaticGun is now TriggerEffectType::Limited_Weapon
     *               -----------------------------------------------------------------------------------
     *               TriggerEffectGenerator.Reset(std::vector<uint8_t>& destinationArray, int destinationIndex) is now TriggerEffectGenerator.Off(std::vector<uint8_t>& destinationArray, int destinationIndex)
     *               TriggerEffectGenerator.Resistance(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t force) is now TriggerEffectGenerator.Feedback(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t strength)
     *               TriggerEffectGenerator.SemiAutomaticGun(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t end, uint8_t force) is now TriggerEffectGenerator.Weapon(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t strength)
     *               TriggerEffectGenerator.AutomaticGun(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t strength, uint8_t frequency) is now TriggerEffectGenerator.Vibration(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t amplitude, uint8_t frequency)
     *               -----------------------------------------------------------------------------------
     *               TriggerEffectGenerator.Bow(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t end, uint8_t force, uint8_t snapForce) is now TriggerEffectGenerator.
     *               TriggerEffectGenerator.Galloping(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t end, uint8_t firstFoot, uint8_t secondFoot, uint8_t frequency) is now TriggerEffectGenerator.Galloping(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t firstFoot, uint8_t secondFoot, uint8_t frequency)
     *               TriggerEffectGenerator.Machine(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t end, uint8_t strengthA, uint8_t strengthB, uint8_t frequency, uint8_t period) is now TriggerEffectGenerator.Machine(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t amplitudeA, uint8_t amplitudeB, uint8_t frequency, uint8_t period)
     *               -----------------------------------------------------------------------------------
     *               TriggerEffectGenerator.SimpleResistance(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t force) is now TriggerEffectGenerator.Simple_Feedback(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t strength)
     *               TriggerEffectGenerator.SimpleSemiAutomaticGun(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t end, uint8_t force) is now TriggerEffectGenerator.Simple_Weapon(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t strength)
     *               TriggerEffectGenerator.SimpleAutomaticGun(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t strength, uint8_t frequency) is now TriggerEffectGenerator.Simple_Vibration(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t amplitude, uint8_t frequency)
     *               -----------------------------------------------------------------------------------
     *               TriggerEffectGenerator.LimitedResistance(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t force) is now TriggerEffectGenerator.Limited_Feedback(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t strength)
     *               TriggerEffectGenerator.LimitedSemiAutomaticGun(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t start, uint8_t end, uint8_t force) is now TriggerEffectGenerator.Limited_Weapon(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t strength)
     *               -----------------------------------------------------------------------------------
     *               TriggerEffectGenerator.Raw.ResistanceRaw(std::vector<uint8_t>& destinationArray, int destinationIndex, std::vector<uint8_t>& force) is now TriggerEffectGenerator.MultiplePositionFeedback(std::vector<uint8_t>& destinationArray, int destinationIndex, std::vector<uint8_t>& strength)
     *               TriggerEffectGenerator.Raw.AutomaticGunRaw(std::vector<uint8_t>& destinationArray, int destinationIndex, std::vector<uint8_t>& strength, uint8_t frequency) is now TriggerEffectGenerator.MultiplePositionVibration(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t frequency, std::vector<uint8_t>& amplitude)
     * Revision 6: Fixed MultiplePositionVibration not using frequency paramater.
     */

    /// <summary>
    /// DualSense controller trigger effect generators.
    /// Revision: 6
    /// 
    /// If you are converting from offical Sony code you will need to convert your chosen effect enum to its chosen factory
    /// function and your paramater struct to paramaters for that function. Please also note that you will need to track the
    /// controller's currently set effect yourself. Note that all effect factories will return false and not modify the
    /// destinationArray if invalid paramaters are used. If paramaters that would result in zero effect are used, the
    /// <see cref="TriggerEffectType::Off">Off</see> effect is applied instead in line with Sony's offical behavior.
    /// All Unofficial, simple, and limited effects are defined as close to the offical effect implementations as possible.
    /// </summary>
    namespace TriggerEffectGenerator {
        /// <summary>
        /// Turn the trigger effect off and return the trigger stop to the neutral position.
        /// This is an offical effect and is expected to be present in future DualSense firmware.
        /// </summary>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <returns>The success of the effect write.</returns>
        bool Off(std::vector<uint8_t>& destinationArray, int destinationIndex) {
            destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Off;
            destinationArray[destinationIndex +  1] = 0x00;
            destinationArray[destinationIndex +  2] = 0x00;
            destinationArray[destinationIndex +  3] = 0x00;
            destinationArray[destinationIndex +  4] = 0x00;
            destinationArray[destinationIndex +  5] = 0x00;
            destinationArray[destinationIndex +  6] = 0x00;
            destinationArray[destinationIndex +  7] = 0x00;
            destinationArray[destinationIndex +  8] = 0x00;
            destinationArray[destinationIndex +  9] = 0x00;
            destinationArray[destinationIndex + 10] = 0x00;
            return true;
        }

        /// <summary>
        /// Trigger will resist movement beyond the start position.
        /// The trigger status nybble will report 0 before the effect and 1 when in the effect.
        /// This is an offical effect and is expected to be present in future DualSense firmware.
        /// </summary>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="position">The starting zone of the trigger effect. Must be between 0 and 9 inclusive.</param>
        /// <param name="strength">The force of the resistance. Must be between 0 and 8 inclusive.</param>
        /// <returns>The success of the effect write.</returns>
        bool Feedback(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t strength)
        {
            if (position > 9)
                return false;
            if (strength > 8)
                return false;
            if (strength > 0)
            {
                uint8_t forceValue = (uint8_t)((strength - 1) & 0x07);
                uint32_t forceZones  = 0;
                uint16_t activeZones = 0;
                for (int i = position; i < 10; i++)
                {
                    forceZones  |= (uint32_t)(forceValue << (3 * i));
                    activeZones |= (uint16_t)(1 << i);
                }

                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Feedback;
                destinationArray[destinationIndex +  1] = (uint8_t)((activeZones >> 0) & 0xff);
                destinationArray[destinationIndex +  2] = (uint8_t)((activeZones >> 8) & 0xff);
                destinationArray[destinationIndex +  3] = (uint8_t)((forceZones >>  0) & 0xff);
                destinationArray[destinationIndex +  4] = (uint8_t)((forceZones >>  8) & 0xff);
                destinationArray[destinationIndex +  5] = (uint8_t)((forceZones >> 16) & 0xff);
                destinationArray[destinationIndex +  6] = (uint8_t)((forceZones >> 24) & 0xff);
                destinationArray[destinationIndex +  7] = 0x00; // (uint8_t)((forceZones >> 32) & 0xff); // need 64bit for this, but we already have enough space
                destinationArray[destinationIndex +  8] = 0x00; // (uint8_t)((forceZones >> 40) & 0xff); // need 64bit for this, but we already have enough space
                destinationArray[destinationIndex +  9] = 0x00;
                destinationArray[destinationIndex + 10] = 0x00;
                return true;
            }

            return Off(destinationArray, destinationIndex);
        }

        /// <summary>
        /// Trigger will resist movement beyond the start position until the end position.
        /// The trigger status nybble will report 0 before the effect and 1 when in the effect,
        /// and 2 after until again before the start position.
        /// This is an offical effect and is expected to be present in future DualSense firmware.
        /// </summary>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="startPosition">The starting zone of the trigger effect. Must be between 2 and 7 inclusive.</param>
        /// <param name="endPosition">The ending zone of the trigger effect. Must be between <paramref name="startPosition"/>+1 and 8 inclusive.</param>
        /// <param name="strength">The force of the resistance. Must be between 0 and 8 inclusive.</param>
        /// <returns>The success of the effect write.</returns>
        bool Weapon(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t strength) {
            if (startPosition > 7 || startPosition < 2)
                return false;
            if (endPosition > 8)
                return false;
            if (endPosition <= startPosition)
                return false;
            if (strength > 8)
                return false;
            if (strength > 0)
            {
                uint16_t startAndStopZones = (uint16_t)((1 << startPosition) | (1 << endPosition));

                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Weapon;
                destinationArray[destinationIndex +  1] = (uint8_t)((startAndStopZones >> 0) & 0xff);
                destinationArray[destinationIndex +  2] = (uint8_t)((startAndStopZones >> 8) & 0xff);
                destinationArray[destinationIndex +  3] = (uint8_t)(strength - 1); // this is actually packed into 3 bits, but since it's only one why bother with the fancy code?
                destinationArray[destinationIndex +  4] = 0x00;
                destinationArray[destinationIndex +  5] = 0x00;
                destinationArray[destinationIndex +  6] = 0x00;
                destinationArray[destinationIndex +  7] = 0x00;
                destinationArray[destinationIndex +  8] = 0x00;
                destinationArray[destinationIndex +  9] = 0x00;
                destinationArray[destinationIndex + 10] = 0x00;
                return true;
            }

            return Off(destinationArray, destinationIndex);
        }
        /// <summary>
        /// Trigger will vibrate with the input amplitude and frequency beyond the start position.
        /// The trigger status nybble will report 0 before the effect and 1 when in the effect.
        /// This is an offical effect and is expected to be present in future DualSense firmware.
        /// </summary>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="position">The starting zone of the trigger effect. Must be between 0 and 9 inclusive.</param>
        /// <param name="amplitude">Strength of the automatic cycling action. Must be between 0 and 8 inclusive.</param>
        /// <param name="frequency">Frequency of the automatic cycling action in hertz.</param>
        /// <returns>The success of the effect write.</returns>
        bool Vibration(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t amplitude, uint8_t frequency) {
            if (position > 9)
                return false;
            if (amplitude > 8)
                return false;
            if (amplitude > 0 && frequency > 0)
            {
                uint8_t strengthValue = (uint8_t)((amplitude - 1) & 0x07);
                uint32_t amplitudeZones = 0;
                uint16_t activeZones    = 0;
                for (int i = position; i < 10; i++)
                {
                    amplitudeZones |= (uint32_t)(strengthValue << (3 * i));
                    activeZones   |= (uint16_t)(1 << i);
                }

                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Vibration;
                destinationArray[destinationIndex +  1] = (uint8_t)((activeZones    >>  0) & 0xff);
                destinationArray[destinationIndex +  2] = (uint8_t)((activeZones    >>  8) & 0xff);
                destinationArray[destinationIndex +  3] = (uint8_t)((amplitudeZones >>  0) & 0xff);
                destinationArray[destinationIndex +  4] = (uint8_t)((amplitudeZones >>  8) & 0xff);
                destinationArray[destinationIndex +  5] = (uint8_t)((amplitudeZones >> 16) & 0xff);
                destinationArray[destinationIndex +  6] = (uint8_t)((amplitudeZones >> 24) & 0xff);
                destinationArray[destinationIndex +  7] = 0x00; // (uint8_t)((strengthZones >> 32) & 0xff); // need 64bit for this, but we already have enough space
                destinationArray[destinationIndex +  8] = 0x00; // (uint8_t)((strengthZones >> 40) & 0xff); // need 64bit for this, but we already have enough space
                destinationArray[destinationIndex +  9] = frequency;
                destinationArray[destinationIndex + 10] = 0x00;
                return true;
            }
            return Off(destinationArray, destinationIndex);
        }

        /// <summary>
        /// Trigger will resist movement at varrying strengths in 10 regions.
        /// This is an offical effect and is expected to be present in future DualSense firmware.
        /// </summary>
        /// <seealso cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="strength">Array of 10 resistance values for zones 0 through 9. Must be between 0 and 8 inclusive.</param>
        /// <returns>The success of the effect write.</returns>
        bool MultiplePositionFeedback(std::vector<uint8_t>& destinationArray, int destinationIndex, std::vector<uint8_t>& strength) {
            if (strength.size() != 10) return false;
            
            if(std::find_if(strength.begin(), strength.end(), [](float x) { return x > 0; }) != strength.end()) {
                uint32_t forceZones  = 0;
                uint16_t activeZones = 0;
                for (int i = 0; i < 10; i++)
                {
                    if (strength[i] > 0)
                    {
                        uint8_t forceValue = (uint8_t)((strength[i] - 1) & 0x07);
                        forceZones  |= (uint32_t)(forceValue << (3 * i));
                        activeZones |= (uint16_t)(1 << i);
                    }
                }

                destinationArray[destinationIndex + 0] = (uint8_t)TriggerEffectType::Feedback;
                destinationArray[destinationIndex + 1] = (uint8_t)((activeZones >> 0) & 0xff);
                destinationArray[destinationIndex + 2] = (uint8_t)((activeZones >> 8) & 0xff);
                destinationArray[destinationIndex + 3] = (uint8_t)((forceZones >>  0) & 0xff);
                destinationArray[destinationIndex + 4] = (uint8_t)((forceZones >>  8) & 0xff);
                destinationArray[destinationIndex + 5] = (uint8_t)((forceZones >> 16) & 0xff);
                destinationArray[destinationIndex + 6] = (uint8_t)((forceZones >> 24) & 0xff);
                destinationArray[destinationIndex + 7] = 0x00; // (uint8_t)((forceZones >> 32) & 0xff); // need 64bit for this, but we already have enough space
                destinationArray[destinationIndex + 8] = 0x00; // (uint8_t)((forceZones >> 40) & 0xff); // need 64bit for this, but we already have enough space
                destinationArray[destinationIndex + 9] = 0x00;
                destinationArray[destinationIndex + 10] = 0x00;
                return true;
            }
            return Off(destinationArray, destinationIndex);
        }

        /// <summary>
        /// Trigger will resist movement at a linear range of strengths.
        /// This is an offical effect and is expected to be present in future DualSense firmware.
        /// </summary>
        /// <seealso cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="startPosition">The starting zone of the trigger effect. Must be between 0 and 8 inclusive.</param>
        /// <param name="endPosition">The ending zone of the trigger effect. Must be between <paramref name="startPosition"/>+1 and 9 inclusive.</param>
        /// <param name="startStrength">The force of the resistance at the start. Must be between 1 and 8 inclusive.</param>
        /// <param name="endStrength">The force of the resistance at the end. Must be between 1 and 8 inclusive.</param>
        /// <returns>The success of the effect write.</returns>
        bool SlopeFeedback(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t startStrength, uint8_t endStrength) {
            if (startPosition > 8 || startPosition < 0)
                return false;
            if (endPosition > 9)
                return false;
            if (endPosition <= startPosition)
                return false;
            if (startStrength > 8)
                return false;
            if (startStrength < 1)
                return false;
            if (endStrength > 8)
                return false;
            if (endStrength < 1)
                return false;

            std::vector<uint8_t> strength(10);
            float slope = 1.0f * (endStrength - startStrength) / (endPosition - startPosition);
            for (int i = (int)startPosition; i < 10; i++)
                if (i <= endPosition)
                    strength[i] = (uint8_t)round(startStrength + slope * (i - startPosition));
                else
                    strength[i] = endStrength;

            return MultiplePositionFeedback(destinationArray, destinationIndex, strength);
        }

        /// <summary>
        /// Trigger will vibrate movement at varrying amplitudes and one frequency in 10 regions.
        /// This is an offical effect and is expected to be present in future DualSense firmware.
        /// </summary>
        /// <remarks>
        /// Note this factory's results may not perform as expected.
        /// </remarks>
        /// <seealso cref="Vibration(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="amplitude">Array of 10 strength values for zones 0 through 9. Must be between 0 and 8 inclusive.</param>
        /// <param name="frequency">Frequency of the automatic cycling action in hertz.</param>
        /// <returns>The success of the effect write.</returns>
        bool MultiplePositionVibration(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t frequency, std::vector<uint8_t>& amplitude) {
            if (amplitude.size() != 10) return false;

            if (frequency > 0 && (std::find_if(amplitude.begin(), amplitude.end(), [](uint8_t x) { return x > 0; }) != amplitude.end()))
            {
                uint32_t strengthZones = 0;
                uint16_t activeZones   = 0;
                for (int i = 0; i < 10; i++)
                {
                    if (amplitude[i] > 0)
                    {
                        uint8_t strengthValue = (uint8_t)((amplitude[i] - 1) & 0x07);
                        strengthZones |= (uint32_t)(strengthValue << (3 * i));
                        activeZones   |= (uint16_t)(1 << i);
                    }
                }

                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Vibration;
                destinationArray[destinationIndex +  1] = (uint8_t)((activeZones >> 0) & 0xff);
                destinationArray[destinationIndex +  2] = (uint8_t)((activeZones >> 8) & 0xff);
                destinationArray[destinationIndex +  3] = (uint8_t)((strengthZones >>  0) & 0xff);
                destinationArray[destinationIndex +  4] = (uint8_t)((strengthZones >>  8) & 0xff);
                destinationArray[destinationIndex +  5] = (uint8_t)((strengthZones >> 16) & 0xff);
                destinationArray[destinationIndex +  6] = (uint8_t)((strengthZones >> 24) & 0xff);
                destinationArray[destinationIndex +  7] = 0x00; // (uint8_t)((forceZones >> 32) & 0xff); // need 64bit for this, but we already have enough space
                destinationArray[destinationIndex +  8] = 0x00; // (uint8_t)((forceZones >> 40) & 0xff); // need 64bit for this, but we already have enough space
                destinationArray[destinationIndex +  9] = frequency;
                destinationArray[destinationIndex + 10] = 0x00;
                return true;
            }

            return Off(destinationArray, destinationIndex);
        }

        /// <summary>
        /// The effect resembles the <see cref="Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)">Weapon</see>
        /// effect, however there is a snap-back force that attempts to reset the trigger.
        /// This is not an offical effect and may be removed in a future DualSense firmware.
        /// </summary>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="startPosition">The starting zone of the trigger effect. Must be between 0 and 8 inclusive.</param>
        /// <param name="endPosition">The ending zone of the trigger effect. Must be between <paramref name="startPosition"/>+1 and 8 inclusive.</param>
        /// <param name="strength">The force of the resistance. Must be between 0 and 8 inclusive.</param>
        /// <param name="snapForce">The force of the snap-back. Must be between 0 and 8 inclusive.</param>
        /// <returns>The success of the effect write.</returns>
        bool Bow(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t strength, uint8_t snapForce) {
            if (startPosition > 8)
                return false;
            if (endPosition > 8)
                return false;
            if (startPosition >= endPosition)
                return false;
            if (strength > 8)
                return false;
            if (snapForce > 8)
                return false;
            if (endPosition > 0 && strength > 0 && snapForce > 0)
            {
                uint16_t startAndStopZones = (uint16_t)((1 << startPosition) | (1 << endPosition));
                uint32_t forcePair = (uint32_t)((((strength  - 1) & 0x07) << (3 * 0))
                                          | (((snapForce - 1) & 0x07) << (3 * 1)));

                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Bow;
                destinationArray[destinationIndex +  1] = (uint8_t)((startAndStopZones >> 0) & 0xff);
                destinationArray[destinationIndex +  2] = (uint8_t)((startAndStopZones >> 8) & 0xff);
                destinationArray[destinationIndex +  3] = (uint8_t)((forcePair >> 0) & 0xff);
                destinationArray[destinationIndex +  4] = (uint8_t)((forcePair >> 8) & 0xff);
                destinationArray[destinationIndex +  5] = 0x00;
                destinationArray[destinationIndex +  6] = 0x00;
                destinationArray[destinationIndex +  7] = 0x00;
                destinationArray[destinationIndex +  8] = 0x00;
                destinationArray[destinationIndex +  9] = 0x00;
                destinationArray[destinationIndex + 10] = 0x00;
                return true;
            }

            return Off(destinationArray, destinationIndex);
        }

        /// <summary>
        /// Trigger will oscillate in a rythmic pattern resembling galloping. Note that the
        /// effect is only discernable at low frequency values.
        /// This is not an offical effect and may be removed in a future DualSense firmware.
        /// </summary>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="startPosition">The starting zone of the trigger effect. Must be between 0 and 8 inclusive.</param>
        /// <param name="endPosition">The ending zone of the trigger effect. Must be between <paramref name="startPosition"/>+1 and 9 inclusive.</param>
        /// <param name="firstFoot">Position of second foot in cycle. Must be between 0 and 6 inclusive.</param>
        /// <param name="secondFoot">Position of second foot in cycle. Must be between <paramref name="firstFoot"/>+1 and 7 inclusive.</param>
        /// <param name="frequency">Frequency of the automatic cycling action in hertz.</param>
        /// <returns>The success of the effect write.</returns>
        bool Galloping(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t firstFoot, uint8_t secondFoot, uint8_t frequency) {
            if (startPosition > 8)
                return false;
            if (endPosition > 9)
                return false;
            if (startPosition >= endPosition)
                return false;
            if (secondFoot > 7)
                return false;
            if (firstFoot > 6)
                return false;
            if (firstFoot >= secondFoot)
                return false;
            if (frequency > 0)
            {
                uint16_t startAndStopZones = (uint16_t)((1 << startPosition) | (1 << endPosition));
                uint32_t timeAndRatio = (uint32_t)(((secondFoot & 0x07) << (3 * 0))
                                             | ((firstFoot  & 0x07) << (3 * 1)));

                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Galloping;
                destinationArray[destinationIndex +  1] = (uint8_t)((startAndStopZones >> 0) & 0xff);
                destinationArray[destinationIndex +  2] = (uint8_t)((startAndStopZones >> 8) & 0xff);
                destinationArray[destinationIndex +  3] = (uint8_t)((timeAndRatio >> 0) & 0xff);
                destinationArray[destinationIndex +  4] = frequency; // this is actually packed into 3 bits, but since it's only one why bother with the fancy code?
                destinationArray[destinationIndex +  5] = 0x00;
                destinationArray[destinationIndex +  6] = 0x00;
                destinationArray[destinationIndex +  7] = 0x00;
                destinationArray[destinationIndex +  8] = 0x00;
                destinationArray[destinationIndex +  9] = 0x00;
                destinationArray[destinationIndex + 10] = 0x00;

                return true;
            }

            return Off(destinationArray, destinationIndex);
        }


        /// <summary>
        /// This effect resembles <see cref="Vibration(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)">Vibration</see>
        /// but will oscilate between two amplitudes.
        /// This is not an offical effect and may be removed in a future DualSense firmware.
        /// </summary>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="startPosition">The starting zone of the trigger effect. Must be between 0 and 8 inclusive.</param>
        /// <param name="endPosition">The ending zone of the trigger effect. Must be between <paramref name="startPosition"/> and 9 inclusive.</param>
        /// <param name="amplitudeA">Primary strength of cycling action. Must be between 0 and 7 inclusive.</param>
        /// <param name="amplitudeB">Secondary strength of cycling action. Must be between 0 and 7 inclusive.</param>
        /// <param name="frequency">Frequency of the automatic cycling action in hertz.</param>
        /// <param name="period">Period of the oscillation between <paramref name="amplitudeA"/> and <paramref name="amplitudeB"/> in tenths of a second.</param>
        /// <returns>The success of the effect write.</returns>
        bool Machine(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t amplitudeA, uint8_t amplitudeB, uint8_t frequency, uint8_t period) {
            if (startPosition > 8)
                return false;
            if (endPosition > 9)
                return false;
            if (endPosition <= startPosition)
                return false;
            if (amplitudeA > 7)
                return false;
            if (amplitudeB > 7)
                return false;
            if (frequency > 0)
            {
                uint16_t startAndStopZones = (uint16_t)((1 << startPosition) | (1 << endPosition));
                uint32_t strengthPair = (uint32_t)(((amplitudeA & 0x07) << (3 * 0))
                                             | ((amplitudeB & 0x07) << (3 * 1)));

                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Machine;
                destinationArray[destinationIndex +  1] = (uint8_t)((startAndStopZones >> 0) & 0xff);
                destinationArray[destinationIndex +  2] = (uint8_t)((startAndStopZones >> 8) & 0xff);
                destinationArray[destinationIndex +  3] = (uint8_t)((strengthPair >> 0) & 0xff);
                destinationArray[destinationIndex +  4] = frequency;
                destinationArray[destinationIndex +  5] = period;
                destinationArray[destinationIndex +  6] = 0x00;
                destinationArray[destinationIndex +  7] = 0x00;
                destinationArray[destinationIndex +  8] = 0x00;
                destinationArray[destinationIndex +  9] = 0x00;
                destinationArray[destinationIndex + 10] = 0x00;

                return true;
            }

            return Off(destinationArray, destinationIndex);
        }
        /// <summary>
        /// Simplistic Feedback effect data generator.
        /// This is not an offical effect and has an offical alternative. It may be removed in a future DualSense firmware.
        /// </summary>
        /// <remarks>
        /// Use <see cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/> instead.
        /// </remarks>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="position">The starting zone of the trigger effect.</param>
        /// <param name="strength">The force of the resistance.</param>
        /// <returns>The success of the effect write.</returns>
        bool Simple_Feedback(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t strength) {
            destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Simple_Feedback;
            destinationArray[destinationIndex +  1] = position;
            destinationArray[destinationIndex +  2] = strength;
            destinationArray[destinationIndex +  3] = 0x00;
            destinationArray[destinationIndex +  4] = 0x00;
            destinationArray[destinationIndex +  5] = 0x00;
            destinationArray[destinationIndex +  6] = 0x00;
            destinationArray[destinationIndex +  7] = 0x00;
            destinationArray[destinationIndex +  8] = 0x00;
            destinationArray[destinationIndex +  9] = 0x00;
            destinationArray[destinationIndex + 10] = 0x00;
            return true;
        }

        /// <summary>
        /// Simplistic Weapon effect data generator.
        /// This is not an offical effect and has an offical alternative. It may be removed in a future DualSense firmware.
        /// </summary>
        /// <remarks>
        /// Use <see cref="Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/> instead.
        /// </remarks>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="startPosition">The starting zone of the trigger effect.</param>
        /// <param name="endPosition">The ending zone of the trigger effect.</param>
        /// <param name="strength">The force of the resistance.</param>
        /// <returns>The success of the effect write.</returns>
        bool Simple_Weapon(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t strength) {
            destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Simple_Weapon;
            destinationArray[destinationIndex +  1] = startPosition;
            destinationArray[destinationIndex +  2] = endPosition;
            destinationArray[destinationIndex +  3] = strength;
            destinationArray[destinationIndex +  4] = 0x00;
            destinationArray[destinationIndex +  5] = 0x00;
            destinationArray[destinationIndex +  6] = 0x00;
            destinationArray[destinationIndex +  7] = 0x00;
            destinationArray[destinationIndex +  8] = 0x00;
            destinationArray[destinationIndex +  9] = 0x00;
            destinationArray[destinationIndex + 10] = 0x00;
            return true;
        }

        /// <summary>
        /// Simplistic Vibration effect data generator.
        /// This is not an offical effect and has an offical alternative. It may be removed in a future DualSense firmware.
        /// </summary>
        /// <remarks>
        /// Use <see cref="Vibration(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/> instead.
        /// </remarks>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="position">The starting zone of the trigger effect.</param>
        /// <param name="amplitude">Strength of the automatic cycling action.</param>
        /// <param name="frequency">Frequency of the automatic cycling action in hertz.</param>
        /// <returns>The success of the effect write.</returns>
        bool Simple_Vibration(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t amplitude, uint8_t frequency) {
            if (frequency > 0 && amplitude > 0)
            {
                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Simple_Vibration;
                destinationArray[destinationIndex +  1] = frequency;
                destinationArray[destinationIndex +  2] = amplitude;
                destinationArray[destinationIndex +  3] = position;
                destinationArray[destinationIndex +  4] = 0x00;
                destinationArray[destinationIndex +  5] = 0x00;
                destinationArray[destinationIndex +  6] = 0x00;
                destinationArray[destinationIndex +  7] = 0x00;
                destinationArray[destinationIndex +  8] = 0x00;
                destinationArray[destinationIndex +  9] = 0x00;
                destinationArray[destinationIndex + 10] = 0x00;
                return true;
            }
            return Off(destinationArray, destinationIndex);
        }

        /// <summary>
        /// Simplistic Feedback effect data generator with stricter paramater limits.
        /// This is not an offical effect and has an offical alternative. It may be removed in a future DualSense firmware.
        /// </summary>
        /// <remarks>
        /// Use <see cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/> instead.
        /// </remarks>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="position">The starting zone of the trigger effect.</param>
        /// <param name="strength">The force of the resistance. Must be between 0 and 10 inclusive.</param>
        /// <returns>The success of the effect write.</returns>
        bool Limited_Feedback(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t position, uint8_t strength) {
            if (strength > 10)
                return false;
            if (strength > 0)
            {
                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Limited_Feedback;
                destinationArray[destinationIndex +  1] = position;
                destinationArray[destinationIndex +  2] = strength;
                destinationArray[destinationIndex +  3] = 0x00;
                destinationArray[destinationIndex +  4] = 0x00;
                destinationArray[destinationIndex +  5] = 0x00;
                destinationArray[destinationIndex +  6] = 0x00;
                destinationArray[destinationIndex +  7] = 0x00;
                destinationArray[destinationIndex +  8] = 0x00;
                destinationArray[destinationIndex +  9] = 0x00;
                destinationArray[destinationIndex + 10] = 0x00;
                return true;
            }
            return Off(destinationArray, destinationIndex);
        }

        /// <summary>
        /// Simplistic Weapon effect data generator with stricter paramater limits.
        /// This is not an offical effect and has an offical alternative. It may be removed in a future DualSense firmware.
        /// </summary>
        /// <remarks>
        /// Use <see cref="Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/> instead.
        /// </remarks>
        /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
        /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
        /// <param name="startPosition">The starting zone of the trigger effect. Must be 16 or higher.</param>
        /// <param name="endPosition">The ending zone of the trigger effect. Must be between <paramref name="startPosition"/> and <paramref name="startPosition"/>+100 inclusive.</param>
        /// <param name="strength">The force of the resistance. Must be between 0 and 10 inclusive.</param>
        /// <returns>The success of the effect write.</returns>
        bool Limited_Weapon(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t startPosition, uint8_t endPosition, uint8_t strength)
        {
            if (startPosition < 0x10)
                return false;
            if (endPosition < startPosition || (startPosition + 100) < endPosition)
                return false;
            if (strength > 10)
                return false;
            if (strength > 0)
            {
                destinationArray[destinationIndex +  0] = (uint8_t)TriggerEffectType::Limited_Weapon;
                destinationArray[destinationIndex +  1] = startPosition;
                destinationArray[destinationIndex +  2] = endPosition;
                destinationArray[destinationIndex +  3] = strength;
                destinationArray[destinationIndex +  4] = 0x00;
                destinationArray[destinationIndex +  5] = 0x00;
                destinationArray[destinationIndex +  6] = 0x00;
                destinationArray[destinationIndex +  7] = 0x00;
                destinationArray[destinationIndex +  8] = 0x00;
                destinationArray[destinationIndex +  9] = 0x00;
                destinationArray[destinationIndex + 10] = 0x00;
                return true;
            }
            return Off(destinationArray, destinationIndex);
        }

        /// <summary>
        /// Interface adapaters patterned after Apple's GCDualSenseAdaptiveTrigger classs.
        /// </summary>
        namespace apple {
            /// <summary>
            /// Sets the adaptive trigger to feedback mode. The start position and strength of the effect can be set arbitrarily. The trigger arm will continue to provide a
            /// constant degree of feedback whenever it is depressed further than the start position.
            /// </summary>
            /// <remarks>
            /// Documentation ported from Apple's API Docs.
            /// </remarks>
            /// <seealso cref="Off(std::vector<uint8_t>&, int)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool SetModeOff(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Off(destinationArray, destinationIndex);
            }

            /// <summary>
            /// Sets the adaptive trigger to feedback mode. The start position and strength of the effect can be set arbitrarily. The trigger arm will continue to provide a
            /// constant degree of feedback whenever it is depressed further than the start position.
            /// </summary>
            /// <remarks>
            /// Documentation ported from Apple's API Docs.
            /// </remarks>
            /// <seealso cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <param name="startPosition">A normalized float from [0-1], with 0 representing the smallest possible trigger depression and 1 representing the maximum trigger depression.</param>
            /// <param name="resistiveStrength">A normalized float from [0-1], with 0 representing the minimum effect strength (off entirely) and 1 representing the maximum effect strength.</param>
            /// <returns>The success of the effect write.</returns>
            bool SetModeFeedbackWithStartPosition(std::vector<uint8_t>& destinationArray, int destinationIndex, float startPosition, float resistiveStrength)
            {
                startPosition = (float)round(startPosition * 9.0f);
                resistiveStrength = (float)round(resistiveStrength * 8.0f);
                return Feedback(destinationArray, destinationIndex, (uint8_t)startPosition, (uint8_t)resistiveStrength);
            }

            /// <summary>
            /// Sets the adaptive trigger to weapon mode. The start position, end position, and strength of the effect can be set arbitrarily; however the end position must be larger than the start position.
            /// The trigger arm will continue to provide a constant degree of feedback whenever it is depressed further than the start position. Once the trigger arm has been depressed past the end
            /// position, the strength of the effect will immediately fall to zero, providing a "sense of release" similar to that provided by pulling the trigger of a weapon.
            /// </summary>
            /// <remarks>
            /// Documentation ported from Apple's API Docs.
            /// </remarks>
            /// <seealso cref="Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <param name="startPosition">A normalized float from [0-1], with 0 representing the smallest possible depression and 1 representing the maximum trigger depression. The effect will begin once the trigger is depressed beyond this point.</param>
            /// <param name="endPosition">A normalized float from [0-1], with 0 representing the smallest possible depression and 1 representing the maximum trigger depression. Must be greater than startPosition. The effect will end once the trigger is depressed beyond this point.</param>
            /// <param name="resistiveStrength">A normalized float from [0-1], with 0 representing the minimum effect strength (off entirely) and 1 representing the maximum effect strength.</param>
            /// <returns>The success of the effect write.</returns>
            bool SetModeWeaponWithStartPosition(std::vector<uint8_t>& destinationArray, int destinationIndex, float startPosition, float endPosition, float resistiveStrength)
            {
                startPosition = (float)round(startPosition * 9.0f);
                endPosition = (float)round(endPosition * 9.0f);
                resistiveStrength = (float)round(resistiveStrength * 8.0f);
                return Weapon(destinationArray, destinationIndex, (uint8_t)startPosition, (uint8_t)endPosition, (uint8_t)resistiveStrength);
            }

            /// <summary>
            /// Sets the adaptive trigger to vibration mode. The start position, amplitude, and frequency of the effect can be set arbitrarily. The trigger arm will continue to strike against
            /// the trigger whenever it is depressed further than the start position, providing a "sense of vibration".
            /// </summary>
            /// <remarks>
            /// Documentation ported from Apple's API Docs.
            /// </remarks>
            /// <seealso cref="Vibration(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <param name="startPosition">A normalized float from [0-1], with 0 representing the smallest possible depression and 1 representing the maximum trigger depression. The effect will begin once the trigger is depressed beyond this point.</param>
            /// <param name="amplitude">A normalized float from [0-1], with 0 representing the minimum effect strength (off entirely) and 1 representing the maximum effect strength.</param>
            /// <param name="frequency">A normalized float from [0-1], with 0 representing the minimum frequency and 1 representing the maximum frequency of the vibration effect.</param>
            /// <returns>The success of the effect write.</returns>
            bool SetModeVibrationWithStartPosition(std::vector<uint8_t>& destinationArray, int destinationIndex, float startPosition, float amplitude, float frequency)
            {
                startPosition = (float)round(startPosition * 9.0f);
                amplitude = (float)round(amplitude * 8.0f);
                frequency = (float)round(frequency * 255.0f);
                return Vibration(destinationArray, destinationIndex, (uint8_t)startPosition, (uint8_t)amplitude, (uint8_t)frequency);
            }

            /// <summary>
            /// Sets the adaptive trigger to feedback mode. The strength of the effect can be set arbitrarily per zone.
            /// This implementation is not confirmed.
            /// </summary>
            /// <remarks>
            /// Documentation ported from Apple's API Docs.
            /// </remarks>
            /// <seealso cref="MultiplePositionFeedback(std::vector<uint8_t>&, int, std::vector<uint8_t>&)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <param name="positionalResistiveStrengths">An array of 10 normalized floats from [0-1], with 0 representing the minimum effect strength (off entirely) and 1 representing the maximum effect strength.</param>
            /// <returns>The success of the effect write.</returns>
            bool SetModeFeedback(std::vector<uint8_t>& destinationArray, int destinationIndex, std::vector<float> positionalResistiveStrengths)
            {
                if (positionalResistiveStrengths.size() != 10) return false;

                std::vector<uint8_t> force(10);
                for (int i = 0; i < 10; i++)
                    force[i] = (uint8_t)round(positionalResistiveStrengths[i] * 8.0f);

                return MultiplePositionFeedback(destinationArray, destinationIndex, force);
            }

            /// <summary>
            /// Sets the adaptive trigger to feedback mode. The strength of the effect will change across zones based on a slope.
            /// This implementation is not confirmed.
            /// </summary>
            /// <remarks>
            /// Documentation ported from Apple's API Docs.
            /// </remarks>
            /// <seealso cref="MultiplePositionFeedback(std::vector<uint8_t>&, int, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <param name="startPosition">A normalized float from [0-1], with 0 representing the smallest possible depression and 1 representing the maximum trigger depression. The effect will begin once the trigger is depressed beyond this point.</param>
            /// <param name="endPosition">A normalized float from [0-1], with 0 representing the smallest possible depression and 1 representing the maximum trigger depression. Must be greater than startPosition. The effect will end once the trigger is depressed beyond this point.</param>
            /// <param name="startStrength">A normalized float from [0-1], with 0 representing the minimum effect strength (off entirely) and 1 representing the maximum effect strength.</param>
            /// <param name="endStrength">A normalized float from [0-1], with 0 representing the minimum effect strength (off entirely) and 1 representing the maximum effect strength.</param>
            /// <returns>The success of the effect write.</returns>
            bool setModeSlopeFeedback(std::vector<uint8_t>& destinationArray, int destinationIndex, float startPosition, float endPosition, float startStrength, float endStrength)
            {
                startPosition = (float)round(startPosition * 9.0f);
                endPosition = (float)round(endPosition * 9.0f);
                startStrength = (float)round(startStrength * 8.0f);
                endStrength = (float)round(endStrength * 8.0f);

                return SlopeFeedback(destinationArray, destinationIndex, (uint8_t)startPosition, (uint8_t)endPosition, (uint8_t)startStrength, (uint8_t)endStrength);
            }

            /// <summary>
            /// Sets the adaptive trigger to vibration mode. The frequency of the effect can be set arbitrarily and the amplitude arbitrarily per zone.
            /// This implementation is not confirmed.
            /// </summary>
            /// <remarks>
            /// Documentation ported from Apple's API Docs.
            /// </remarks>
            /// <seealso cref="MultiplePositionVibration(std::vector<uint8_t>&, int, uint8_t, std::vector<uint8_t>&)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <param name="positionalAmplitudes">An array of 10 normalized floats from [0-1], with 0 representing the minimum effect strength (off entirely) and 1 representing the maximum effect strength.</param>
            /// <param name="frequency">A normalized float from [0-1], with 0 representing the minimum frequency and 1 representing the maximum frequency of the vibration effect.</param>
            /// <returns>The success of the effect write.</returns>
            bool setModeVibration(std::vector<uint8_t>& destinationArray, int destinationIndex, std::vector<float> positionalAmplitudes, float frequency)
            {
                if (positionalAmplitudes.size() != 10) return false;

                frequency = (float)round(frequency * 255.0f);

                std::vector<uint8_t> strength(10);
                for (int i = 0; i < 10; i++)
                    strength[i] = (uint8_t)round(positionalAmplitudes[i] * 8.0f);

                return MultiplePositionVibration(destinationArray, destinationIndex, (uint8_t)frequency, strength);
            }
        }

        /// <summary>
        /// Interface adapaters patterned after reWASD's actual interface.
        /// </summary>
        /// <remarks>
        /// This information is based on sniffing the USB traffic from reWASD. Broken implementations are kept though immaterial inaccuracies are corrected.
        /// </remarks>
        namespace rewasd
        {
            /// <summary>
            /// Full Press trigger stop effect data generator.
            /// </summary>
            /// <remarks>
            /// Uses Simple_Weapon with a start value of 0x90, end value of 0xa0, and a force of 0xff.
            /// </remarks>
            /// <seealso cref="Simple_Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <seealso cref="Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool FullPress(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Simple_Weapon(destinationArray, destinationIndex, 0x90, 0xa0, 0xff);
            }

            /// <summary>
            /// Soft Press trigger stop effect data generator.
            /// </summary>
            /// <remarks>
            /// Uses Simple_Weapon with a start value of 0x70, end value of 0xa0, and a force of 0xff.
            /// </remarks>
            /// <seealso cref="Simple_Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <seealso cref="Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool SoftPress(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Simple_Weapon(destinationArray, destinationIndex, 0x70, 0xa0, 0xff);
            }

            /// <summary>
            /// Medium Press trigger stop effect data generator.
            /// </summary>
            /// <remarks>
            /// Uses Simple_Weapon with a start value of 0x45, end value of 0xa0, and a force of 0xff.
            /// </remarks>
            /// <seealso cref="Simple_Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <seealso cref="Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool MediumPress(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Simple_Weapon(destinationArray, destinationIndex, 0x45, 0xa0, 0xff);
            }

            /// <summary>
            /// Hard Press trigger stop effect data generator.
            /// </summary>
            /// <remarks>
            /// Uses Simple_Weapon with a start value of 0x20, end value of 0xa0, and a force of 0xff.
            /// </remarks>
            /// <seealso cref="Simple_Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <seealso cref="Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool HardPress(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Simple_Weapon(destinationArray, destinationIndex, 0x20, 0xa0, 0xff);
            }

            /// <summary>
            /// Pulse trigger stop effect data generator.
            /// </summary>
            /// <remarks>
            /// Uses Simple_Weapon with a start value of 0x00, end value of 0x00, and a force of 0x00.
            /// </remarks>
            /// <seealso cref="Simple_Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <seealso cref="Weapon(std::vector<uint8_t>&, int, uint8_t, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool Pulse(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Simple_Weapon(destinationArray, destinationIndex, 0x00, 0x00, 0x00);
            }

            /// <summary>
            /// Choppy resistance effect data generator.
            /// </summary>
            /// <remarks>
            /// Abuses Feedback effect to set a resistance in 3 of 10 trigger regions.
            /// </remarks>
            /// <seealso cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool Choppy(std::vector<uint8_t>& destinationArray, int destinationIndex)
            {
                destinationArray[destinationIndex + 0] = (uint8_t)TriggerEffectType::Feedback;
                destinationArray[destinationIndex + 1] = (uint8_t)0x02; // region enables
                destinationArray[destinationIndex + 2] = (uint8_t)0x27; // region enables
                destinationArray[destinationIndex + 3] = (uint8_t)0x18; // reWASD uses 0x1f here, but some bits apply to regions not enabled above
                destinationArray[destinationIndex + 4] = (uint8_t)0x00;
                destinationArray[destinationIndex + 5] = (uint8_t)0x00; // reWASD uses 0x27 here, but some bits apply to regions not enabled above
                destinationArray[destinationIndex + 6] = (uint8_t)0x26;
                destinationArray[destinationIndex + 7] = (uint8_t)0x00;
                destinationArray[destinationIndex + 8] = (uint8_t)0x00;
                destinationArray[destinationIndex + 9] = (uint8_t)0x00;
                destinationArray[destinationIndex + 10] = (uint8_t)0x00;
                return true;
            }

            /// <summary>
            /// Soft Rigidity feedback effect data generator.
            /// </summary>
            /// <remarks>
            /// Uses Simple_Feedback with a start value of 0x00 and a force of 0x00.
            /// </remarks>
            /// <seealso cref="Simple_Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <seealso cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool SoftRigidity(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Simple_Feedback(destinationArray, destinationIndex, 0x00, 0x00);
            }

            /// <summary>
            /// Medium Rigidity feedback effect data generator.
            /// </summary>
            /// <remarks>
            /// Uses Simple_Feedback with a start value of 0x00 and a force of 0x64.
            /// </remarks>
            /// <seealso cref="Simple_Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <seealso cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool MediumRigidity(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Simple_Feedback(destinationArray, destinationIndex, 0x00, 0x64);
            }

            /// <summary>
            /// Max Rigidity feedback effect data generator.
            /// </summary>
            /// <remarks>
            /// Uses Simple_Feedback with a start value of 0x00 and a force of 0xdc.
            /// </remarks>
            /// <seealso cref="Simple_Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <seealso cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool MaxRigidity(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Simple_Feedback(destinationArray, destinationIndex, 0x00, 0xdc);
            }

            /// <summary>
            /// Half Press feedback effect data generator.
            /// </summary>
            /// <remarks>
            /// Uses Simple_Feedback with a start value of 0x55 and a force of 0x64.
            /// </remarks>
            /// <seealso cref="Simple_Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <seealso cref="Feedback(std::vector<uint8_t>&, int, uint8_t, uint8_t)"/>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <returns>The success of the effect write.</returns>
            bool HalfPress(std::vector<uint8_t>& destinationArray, int destinationIndex) {
                return Simple_Feedback(destinationArray, destinationIndex, 0x55, 0x64);
            }

            /// <summary>
            /// Rifle vibration effect data generator with some wasted bits.
            /// Bad coding from reWASD was faithfully replicated.
            /// </summary>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <param name="frequency">Frequency of the automatic cycling action in hertz. Must be between 2 and 20 inclusive.</param>
            /// <returns>The success of the effect write.</returns>
            bool Rifle(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t frequency = 10)
            {
                if (frequency < 2)
                    return false;
                if (frequency > 20)
                    return false;

                destinationArray[destinationIndex + 0] = (uint8_t)TriggerEffectType::Vibration;
                destinationArray[destinationIndex + 1] = (uint8_t)0x00;
                destinationArray[destinationIndex + 2] = (uint8_t)0x03; // reWASD uses 0xFF here but the top 6 bits are unused
                destinationArray[destinationIndex + 3] = (uint8_t)0x00;
                destinationArray[destinationIndex + 4] = (uint8_t)0x00;
                destinationArray[destinationIndex + 5] = (uint8_t)0x00;
                destinationArray[destinationIndex + 6] = (uint8_t)0x3F; // reWASD uses 0xFF here but the top 2 bits are unused
                destinationArray[destinationIndex + 7] = (uint8_t)0x00;
                destinationArray[destinationIndex + 8] = (uint8_t)0x00;
                destinationArray[destinationIndex + 9] = frequency;
                destinationArray[destinationIndex + 10] = (uint8_t)0x00;
                return true;
            }

            /// <summary>
            /// Vibration vibration effect with incorrect strength handling.
            /// Bad coding from reWASD was faithfully replicated.
            /// </summary>
            /// <param name="destinationArray">The std::vector<uint8_t>& that receives the data.</param>
            /// <param name="destinationIndex">A 32-bit integer that represents the index in the destinationArray at which storing begins.</param>
            /// <param name="strength">Strength of the automatic cycling action. Must be between 1 and 255 inclusive. This is two 3 bit numbers with the remaining 2 high bits unused. Yes, reWASD uses this value incorrectly.</param>
            /// <param name="frequency">Frequency of the automatic cycling action in hertz. Must be between 1 and 255 inclusive.</param>
            /// <returns>The success of the effect write.</returns>
            bool Vibration(std::vector<uint8_t>& destinationArray, int destinationIndex, uint8_t strength = 220, uint8_t frequency = 30)
            {
                if (strength < 1)
                    return false;
                if (frequency < 1)
                    return false;

                destinationArray[destinationIndex + 0] = (uint8_t)TriggerEffectType::Vibration;
                destinationArray[destinationIndex + 1] = (uint8_t)0x00; // reWASD uses 0x1E here but this is invalid and is ignored save for minor glitches
                destinationArray[destinationIndex + 2] = (uint8_t)0x03; // reWASD uses 0xFF here but the top 6 bits are unused
                destinationArray[destinationIndex + 3] = (uint8_t)0x00;
                destinationArray[destinationIndex + 4] = (uint8_t)0x00;
                destinationArray[destinationIndex + 5] = (uint8_t)0x00;
                destinationArray[destinationIndex + 6] = strength; // reWASD maxes at 0xFF here but the top 2 bits are unused
                destinationArray[destinationIndex + 7] = (uint8_t)0x00;
                destinationArray[destinationIndex + 8] = (uint8_t)0x00;
                destinationArray[destinationIndex + 9] = frequency;
                destinationArray[destinationIndex + 10] = (uint8_t)0x00;
                return true;
            }
        }
    }
}
