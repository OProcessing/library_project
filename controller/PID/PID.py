class PIDController:
    def __init__(self, Kp, Ki, Kd, ref):
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd
        self.ref = ref
        self.integral = 0
        self.prev_error = 0

    def update(self, current_value, dt):
        error = self.ref - current_value
        self.integral += error * dt
        derivative = (error - self.prev_error) / dt if dt > 0 else 0

        # PID formula
        output = (self.Kp * error) + (self.Ki * self.integral) + (self.Kd * derivative)

        # Update previous error
        self.prev_error = error

        return output
