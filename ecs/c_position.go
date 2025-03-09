package ecs

type PositionComponent struct {
	X, Y int32
}

func (ecs *ECSManager) AddPositionComponent(entity EntityID, x, y int32) {
	ecs.Components.Positions[entity] = PositionComponent{X: x, Y: y}
	ecs.Components.PositionMask[entity] = true
}
