#ifndef SENSOR_HPP
#define SENSOR_HPP


enum SensorStatus{
    CONNECTED,
    DISCONNECTED
};

template <typename T>
class Sensor{
    private:
        T value;
        T minValue;
        T maxValue;
        SensorStatus status;

    public:
        Sensor(T min, T max) 
            : value(min), minValue(min), maxValue(max),
            status(SensorStatus::DISCONNECTED){
        // Acquire / initialize hardware here.
        // Example: open I2C device or configure ADC channel.
            }

        ~Sensor() {
            // Automatically release/deinitialize owned hardware here.
        }
        bool setValue(T newValue)
        {
            if (newValue>=minValue && newValue<=maxValue)
            {
                value = newValue;
                return true;
            }
            return false;
        }

        T getValue() const {
            return value;
        } 
        
        T getminValue() const{
            return minValue;
        }

        T getmaxValue() const{
            return maxValue;
        }

        void setConnection(SensorStatus state)
        {
            status = state;
        }

        bool isConnected() const{
            return status == SensorStatus::CONNECTED;
        }


        // Sensor(const Sensor&) = delete;
        // Sensor& operator=(const Sensor&) = delete;

        // Sensor(Sensor&&) = delete;
        // Sensor& operator=(Sensor&&) = delete;

};






#endif