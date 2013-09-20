/*
 * Copyright 2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.0
import "RenderRules.js" as RenderRules

NavigationPane {
    id: loginNavigationPane
    Page {
        Container {
            id: mainContainer
            background: Color.Black
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill     
            
            CarouselDotsControl {
                id: dotsControl
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
            }
            
            Container {
                //This is the button container at the bottom
                leftPadding: RenderRules.StandardListItemPadding()
                rightPadding: RenderRules.StandardListItemPadding()
                bottomPadding: RenderRules.StandardListItemPadding()
                id: buttonContainer
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Button {
                    id: registerButton
                    objectName: "registerButton"
                    text: "Register"
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked: {
                    }	
                }
                Button {
                    id: logInButton
                    objectName: "loginButton"
                    text: "Log In"
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked: {
                        _loginState.loginRequest()
                    }
                }

            }
        } //MainContainer
		

    } //Page
}//NavigationPane
