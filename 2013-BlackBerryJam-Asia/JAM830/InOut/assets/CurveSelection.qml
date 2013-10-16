/* Copyright (c) 2013 BlackBerry Limited.
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
import bb.cascades 1.2

// A RadioGroup placed in a scroll view is used to select which stock curve 
// should be used to run the animation.
ScrollView {
    signal curveChanged(variant curve);
    
    RadioGroup {
        Option {
            text: "Linear"
            selected: true;
            onSelectedChanged: {
                if (selected) {
                    curveChanged(StockCurve.Linear);
                }
            }
        }
        Option {
            text: "QuarticOut"
            onSelectedChanged: {
                if (selected) {
                    curveChanged(StockCurve.QuarticOut);
                }
            }
        }
        Option {
            text: "QuarticIn"
            onSelectedChanged: {
                if (selected) {
                    curveChanged(StockCurve.QuarticIn);
                }
            }
        }
        Option {
            text: "QuarticInOut"
            onSelectedChanged: {
                if (selected) {
                    curveChanged(StockCurve.QuarticInOut);
                }
            }
        }
        Option {
            text: "SineIn"
            onSelectedChanged: {
                if (selected) {
                    curveChanged(StockCurve.SineIn);
                }
            }
        }
        Option {
            text: "BackOut"
            onSelectedChanged: {
                if (selected) {
                    curveChanged(StockCurve.BackOut);
                }
            }
        }
        Option {
            text: "BackInOut"
            onSelectedChanged: {
                if (selected) {
                    curveChanged(StockCurve.BackInOut);
                }
            }
        }
        Option {
            text: "BounceInOut"
            onSelectedChanged: {
                if (selected) {
                    curveChanged(StockCurve.BounceInOut);
                }
            }
        }

    }
}
