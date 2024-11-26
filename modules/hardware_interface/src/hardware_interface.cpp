#include "hardware_interface.hpp"

HardwareInterface::HardwareInterface():initialized(false), need_reset(false), has_update(false), type(GamepadType::NONE), dwAxes(0), dwButtons(0), xinput_index(-1){
    
}

HardwareInterface::~HardwareInterface(){
    shutdown();
}

GamepadData HardwareInterface::getState(){
    // To figure out
    // If an error is encountered, set the initialized flag to false
    return GamepadData();
}

bool HardwareInterface::initDInput(){
    #ifdef _WIN32
    HRESULT hr;
    LPDIRECTINPUT8 pDI;

    if(FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDI, NULL))){
        return false;
    }

    IDirectInputJoyConfig8* pJoyConfig = NULL;

    if(FAILED(hr = pDI->QueryInterface(IID_IDirectInputJoyConfig8, (void **) &pJoyConfig))){
        pDI->Release();
        return false;
    }

    DIJOYCONFIG PreferredJoyCfg = {0};
    PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
    if(FAILED(hr = pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE))){
        pDI->Release();
        if(pJoyConfig){
            pJoyConfig->Release();
        }
        return false;
    }

    if(pJoyConfig){
        pJoyConfig->Release();
    }

    if(FAILED(hr = pDI->CreateDevice(PreferredJoyCfg.guidInstance, &dinputDevice, NULL))){
        pDI->Release();
        return false;
    }
    pDI->Release();
    DIDEVCAPS caps = {sizeof(DIDEVCAPS_DX3)};
    if(FAILED(hr = dinputDevice->GetCapabilities(&caps))){
        dinputDevice->Unacquire();
        return false;
    }
    dwAxes = caps.dwAxes;
    dwButtons = caps.dwButtons;

    if(FAILED(hr = dinputDevice->SetDataFormat(&c_dfDIJoystick))){
        dinputDevice->Unacquire();
        return false;
    }

    if(FAILED(hr = dinputDevice->Acquire())){
        dinputDevice->Unacquire();
        return false;
    }

    #else

    js_fd = open("/dev/input/js0", O_RDONLY);
    if (js_fd == -1){
        return false;
    }

    char number_of_axes;
    char number_of_buttons;
    if(ioctl(js_fd, JSIOCGAXES, &number_of_axes) == -1){
        close(js_fd);
        return false;
    }
    if(ioctl(js_fd, JSIOCGBUTTONS, &number_of_buttons) == -1){
        close(js_fd);
        return false;
    }
    dwAxes = number_of_axes;
    dwButtons = number_of_buttons;

    int flags = fcntl(js_fd, F_GETFL, 0);
    if(fcntl(js_fd, F_SETFL, flags | O_NONBLOCK) == -1){
        close(js_fd);
        return false;
    }

    #endif

    type = GamepadType::DINPUT;
    initialized = true;
    return true;
}

bool HardwareInterface::initXInput(){
    #ifdef _WIN32
    XINPUT_CAPABILITIES cap;
    for(uint8_t i = 0u; i < 4u; i++){
        HRESULT hr = XInputGetCapabilities(i, 0, &cap);
        if(hr == 0){
            xinput_index = i;
            break;
        }
    }
    if(xinput_index == -1){
        return false;
    }

    type = GamepadType::XINPUT;
    initialized = true;
    return true;
    #else
    return false;
    #endif
}

void HardwareInterface::init(){
    if(!initDInput()){
        initXInput();
    }
}

void HardwareInterface::reset(){
    shutdown();
    init();
    latest_state = GamepadData();
    has_update = false;
    if(isInitialized()){
        need_reset = false;
    }
}

void HardwareInterface::shutdown(){    
    if(type == GamepadType::DINPUT){
        #ifdef _WIN32
        dinputDevice->Unacquire();
        #else
        close(js_fd);
        #endif
    }
    else if(type == GamepadType::XINPUT){

    }
    type = GamepadType::NONE;
    initialized = false;
    dwAxes = 0;
    dwButtons = 0;
}

bool HardwareInterface::catchUpdate(){
    if(has_update){
        has_update = false;
        return true;
    }
    return false;
}

#ifdef _WIN32
#else

void HardwareInterface::handleAx8(const js_event& e){
    if(e.type == JS_EVENT_AXIS){
        switch (e.number){
            case 0:
                latest_state.thumb_left_x = (e.value + 0x8000) / 0x100u;
                break;
            
            case 1:
                latest_state.thumb_left_y = (e.value + 0x8000) / 0x100u;
                break;
            
            case 2:
                latest_state.left_trigger = (e.value + 0x8000) / 0x100u;
                break;
            
            case 3:
                latest_state.thumb_right_x= (e.value + 0x8000) / 0x100u;
                break;
            
            case 4:
                latest_state.thumb_right_y = (e.value + 0x8000) / 0x100u;
                break;
            
            case 5:
                latest_state.right_trigger = (e.value + 0x8000) / 0x100u;
                break;
            
            case 6:
                latest_state.l_left = e.value < 0;
                latest_state.l_right = e.value > 0;
                break;
            
            case 7:
                latest_state.l_top = e.value < 0;
                latest_state.l_bottom = e.value > 0;
                break;
            
            default:
                break;
        }
    }
    else if(e.type == JS_EVENT_BUTTON){
        switch (e.number){
            case 0:
                latest_state.r_bottom = e.value;
                break;
            
            case 1:
                latest_state.r_right = e.value;
                break;
            
            case 2:
                latest_state.r_top = e.value;
                break;
            
            case 3:
                latest_state.r_left = e.value;
                break;
            
            case 4:
                latest_state.l_shoulder = e.value;
                break;
            
            case 5:
                latest_state.r_shoulder = e.value;
                break;
            
            default:
                break;
        }
    }
}

void HardwareInterface::handleAx6(const js_event& e){
    if(e.type == JS_EVENT_AXIS){
        switch (e.number){
            case 0:
                latest_state.thumb_left_x = (e.value + 0x8000) / 0x100u;
                break;
            
            case 1:
                latest_state.thumb_left_y = (e.value + 0x8000) / 0x100u;
                break;
            
            case 2:
                latest_state.thumb_right_x = (e.value + 0x8000) / 0x100u;
                break;
            
            case 3:
                latest_state.thumb_right_y = (e.value + 0x8000) / 0x100u;
                break;
            
            case 4:                
                latest_state.l_left = e.value < 0;
                latest_state.l_right = e.value > 0;
                break;
            
            case 5:
                latest_state.l_top = e.value < 0;
                latest_state.l_bottom = e.value > 0;
                break;
            
            default:
                break;
        }
    }
    else if(e.type == JS_EVENT_BUTTON){
        switch (e.number){
            case 0:
                latest_state.r_left = e.value;
                break;
            
            case 1:
                latest_state.r_bottom = e.value;
                break;
            
            case 2:
                latest_state.r_right = e.value;
                break;
            
            case 3:
                latest_state.r_top = e.value;
                break;
            
            case 4:
                latest_state.l_shoulder = e.value;
                break;
            
            case 5:
                latest_state.r_shoulder = e.value;
                break;
            
            case 6:
                latest_state.left_trigger = e.value * 0xFFu;
                break;
            
            case 7:
                latest_state.right_trigger = e.value * 0xFFu;
                break;

            default:
                break;
        }
    }
}

#endif

GamepadData HardwareInterface::poll(){
    if(type == GamepadType::DINPUT){
        #ifdef _WIN32
        DIJOYSTATE newState;
        HRESULT hr;
        if(FAILED(hr = dinputDevice->GetDeviceState(sizeof(DIJOYSTATE), &newState))){
            need_reset = true;
        }
        has_update = latest_state.update(newState, dwAxes, dwButtons);
        #else

        js_event event;
        while(read(js_fd, &event, sizeof(js_event)) == sizeof(js_event)){
            if(dwAxes == 8){
                handleAx8(event);
                has_update = true;
            }
            else if(dwAxes == 6){
                handleAx6(event);
                has_update = true;
            }
		}
        
		if (errno != EAGAIN) {
			need_reset = true;
		}
        #endif
        
    }
    else if (type == GamepadType::XINPUT){
        // both our controllers are dinput it seems
        //has_update = latest_state.update(getState());
    }
    
    return latest_state;
}