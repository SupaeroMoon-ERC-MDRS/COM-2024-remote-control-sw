#include "hardware_interface.hpp"

HardwareInterface::HardwareInterface():initialized(false), need_reset(false), has_update(false), type(GamepadType::NONE), dwAxes(0), dwButtons(0), xinput_index(-1){
    
}

HardwareInterface::~HardwareInterface(){
    close();
}

GamepadData HardwareInterface::getState(){
    // To figure out
    // If an error is encountered, set the initialized flag to false
    return GamepadData();
}

bool HardwareInterface::initDInput(){
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

    type = GamepadType::DINPUT;
    initialized = true;
    return true;
}

bool HardwareInterface::initXInput(){
    // Initialize whatever resources that are required to access the controller
    // if successful, set initialized flag to true 
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
}

void HardwareInterface::init(){
    if(!initDInput()){
        initXInput();
    }
}

void HardwareInterface::reset(){
    close();
    init();
    latest_state = GamepadData();
    has_update = false;
    if(isInitialized()){
        need_reset = false;
    }
}

void HardwareInterface::close(){    
    if(type == GamepadType::DINPUT){
        dinputDevice->Unacquire();
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

GamepadData HardwareInterface::poll(){
    if(type == GamepadType::DINPUT){
        DIJOYSTATE newState;
        HRESULT hr;
        if(FAILED(hr = dinputDevice->GetDeviceState(sizeof(DIJOYSTATE), &newState))){
            need_reset = true;
        }
        has_update = latest_state.update(newState, dwAxes, dwButtons);
        
    }
    else if (type == GamepadType::XINPUT){
        // both our controllers are dinput it seems
        //has_update = latest_state.update(getState());
    }
    
    return latest_state;
}