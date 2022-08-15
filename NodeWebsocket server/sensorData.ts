export class SensorData {
    id: string;
    level: number;
    date: string;

  constructor(sensor: SensorData) {
      this.id = sensor.id;
      this.level = sensor.level;
      this.date = sensor.date; 
  }

}