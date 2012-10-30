
/*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef OPT_HPP
#define OPT_HPP

#include <QtCore/QString>

class Opt
{
public:
    Opt(const Opt &opt);
    Opt(const QString &text, const QString &description, const QString &mime, const QString &uri, const QString &data = QString(), const QString &action = QString());

    QString text() const;
    QString mime() const;
    QString uri() const;
    QString data() const;
    QString action() const;
    QString description() const;

private:
    QString m_text;
    QString m_mime;
    QString m_uri;
    QString m_data;
    QString m_action;
    QString m_description;
};

#endif


