package ecs

import (
	"app/utils"
	"github.com/veandco/go-sdl2/sdl"
	"sort"
)

type EntityID int

type ComponentMask map[EntityID]bool

type ComponentArrays struct {
	Positions    map[EntityID]PositionComponent
	Sprites      map[EntityID]SpriteComponent
	Controls     map[EntityID]ControlsComponent
	PositionMask ComponentMask
	SpriteMask   ComponentMask
	ControlsMask ComponentMask
}

type ECSManager struct {
	NextEntityID int
	Entities     []EntityID
	Components   ComponentArrays
}

func NewECSManager() *ECSManager {
	return &ECSManager{
		NextEntityID: 0,
		Entities:     []EntityID{},
		Components: ComponentArrays{
			Positions:    make(map[EntityID]PositionComponent),
			Sprites:      make(map[EntityID]SpriteComponent),
			Controls:     make(map[EntityID]ControlsComponent),
			PositionMask: make(ComponentMask),
			SpriteMask:   make(ComponentMask),
			ControlsMask: make(ComponentMask),
		},
	}
}

func (ecs *ECSManager) CreateEntity() EntityID {
	id := EntityID(ecs.NextEntityID)
	ecs.NextEntityID++
	ecs.Entities = append(ecs.Entities, id)
	return id
}

func (ecs *ECSManager) DestroyEntity(entity EntityID) {
	// remove from component arrays
	delete(ecs.Components.Positions, entity)
	delete(ecs.Components.Sprites, entity)
	delete(ecs.Components.Controls, entity)

	// clear component masks
	delete(ecs.Components.PositionMask, entity)
	delete(ecs.Components.SpriteMask, entity)
	delete(ecs.Components.ControlsMask, entity)

	// remove from entities list (may be slow)
	for i, e := range ecs.Entities {
		if e == entity {
			ecs.Entities = append(ecs.Entities[:i], ecs.Entities[i+1:]...)
			break
		}
	}
}

type RenderItem struct {
	EntityID EntityID
	X        int32
	Y        int32
	Texture  *sdl.Texture
	Width    int
	Height   int
	ZOrder   int
}

func (ecs *ECSManager) renderSystem(renderer *sdl.Renderer) {
	var renderQueue []RenderItem

	for entity := range ecs.Components.PositionMask {
		if !ecs.Components.SpriteMask[entity] {
			continue
		}

		pos := ecs.Components.Positions[entity]
		sprite := ecs.Components.Sprites[entity]

		renderQueue = append(renderQueue, RenderItem{
			EntityID: entity,
			X:        pos.X,
			Y:        pos.Y,
			Texture:  sprite.Texture,
			Width:    sprite.Width,
			Height:   sprite.Height,
			ZOrder:   sprite.ZOrder,
		})
	}

	sort.Slice(renderQueue, func(i, j int) bool {
		return renderQueue[i].ZOrder < renderQueue[j].ZOrder
	})

	for _, item := range renderQueue {
		renderer.Copy(
			item.Texture,
			nil,
			&sdl.Rect{
				X: int32(item.X),
				Y: int32(item.Y),
				W: int32(item.Width),
				H: int32(item.Height),
			})
	}
}

func (ecs *ECSManager) Update(deltaTime float64, settings *utils.T) {
	ecs.MovementSystem(deltaTime, settings.PLAYER_SPEED)
}

func (ecs *ECSManager) Render(renderer *sdl.Renderer) {
	ecs.renderSystem(renderer)
}
