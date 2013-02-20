/*
 * Copyright (c) 2013 Research In Motion.
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
#ifndef GATE_HPP_
#define GATE_HPP_

#include <QString>

class GateFactory;

class Gate {
public:
	Gate(const char *);
	Gate(const QString &);
	Gate(const QString);
	Gate(GateFactory *, const char *);
//	Gate(GateFactory *, const QString &);
	Gate(GateFactory *, const QString);
	virtual ~Gate();

	Gate & log(QString what);

	Gate & log();

	Gate & operator<<(const char *);
	Gate & operator<<(const int);
	Gate & operator<<(const QString &);

private:
	void doDepth();
	void setMessage(GateFactory *, const char * message);

	QString message;

	class GateState * gateState;

};

#endif /* GATE_HPP_ */
