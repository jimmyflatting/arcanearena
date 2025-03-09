package ecs

import (
	"app/utils"
	"github.com/veandco/go-sdl2/sdl"
)

type ControlsComponent struct {
	UpPressed    bool
	DownPressed  bool
	LeftPressed  bool
	RightPressed bool
	APressed     bool
	BPressed     bool
	PausePressed bool
	QuitPressed  bool
}

// AddControlsComponent adds a controls component to an entity
func (ecs *ECSManager) AddControlsComponent(entity EntityID) {
	ecs.Components.Controls[entity] = ControlsComponent{
		UpPressed:    false,
		DownPressed:  false,
		LeftPressed:  false,
		RightPressed: false,
		APressed:     false,
		BPressed:     false,
		PausePressed: false,
		QuitPressed:  false,
	}
	ecs.Components.ControlsMask[entity] = true
}

func (ecs *ECSManager) MovementSystem(deltaTime float64, playerSpeed int32) {
	for entity := range ecs.Components.PositionMask {
		if !ecs.Components.ControlsMask[entity] {
			continue
		}

		pos := ecs.Components.Positions[entity]
		ctrl := ecs.Components.Controls[entity]

		speed := int32(float64(playerSpeed) * deltaTime)
		if speed < 1 {
			speed = 1
		}

		if ctrl.UpPressed {
			pos.Y -= speed
		}
		if ctrl.DownPressed {
			pos.Y += speed
		}
		if ctrl.LeftPressed {
			pos.X -= speed
		}
		if ctrl.RightPressed {
			pos.X += speed
		}

		ecs.Components.Positions[entity] = pos
	}
}

func (ecs *ECSManager) HandleInput(event sdl.Event, settings *utils.T) bool {
	running := true

	switch e := event.(type) {
	case *sdl.QuitEvent:
		running = false

	case *sdl.KeyboardEvent:
		for entity := range ecs.Components.ControlsMask {
			ctrl := ecs.Components.Controls[entity]

			if e.Type == sdl.KEYDOWN {
				switch e.Keysym.Sym {
				case sdl.GetKeyFromName(settings.CONTROLS.UP):
					ctrl.UpPressed = true
				case sdl.GetKeyFromName(settings.CONTROLS.DOWN):
					ctrl.DownPressed = true
				case sdl.GetKeyFromName(settings.CONTROLS.LEFT):
					ctrl.LeftPressed = true
				case sdl.GetKeyFromName(settings.CONTROLS.RIGHT):
					ctrl.RightPressed = true
				case sdl.GetKeyFromName(settings.CONTROLS.A):
					ctrl.APressed = true
				case sdl.GetKeyFromName(settings.CONTROLS.B):
					ctrl.BPressed = true
				case sdl.GetKeyFromName(settings.CONTROLS.PAUSE):
					ctrl.PausePressed = true
				case sdl.GetKeyFromName(settings.CONTROLS.QUIT):
					ctrl.QuitPressed = true
					running = false
				}
			}

			if e.Type == sdl.KEYUP {
				switch e.Keysym.Sym {
				case sdl.GetKeyFromName(settings.CONTROLS.UP):
					ctrl.UpPressed = false
				case sdl.GetKeyFromName(settings.CONTROLS.DOWN):
					ctrl.DownPressed = false
				case sdl.GetKeyFromName(settings.CONTROLS.LEFT):
					ctrl.LeftPressed = false
				case sdl.GetKeyFromName(settings.CONTROLS.RIGHT):
					ctrl.RightPressed = false
				case sdl.GetKeyFromName(settings.CONTROLS.A):
					ctrl.APressed = false
				case sdl.GetKeyFromName(settings.CONTROLS.B):
					ctrl.BPressed = false
				case sdl.GetKeyFromName(settings.CONTROLS.PAUSE):
					ctrl.PausePressed = false
				case sdl.GetKeyFromName(settings.CONTROLS.QUIT):
					ctrl.QuitPressed = false
				}
			}

			ecs.Components.Controls[entity] = ctrl
		}
	}

	return running
}
